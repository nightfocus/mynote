//
// Created by admin on 2016/12/21.
// 此文件基于 simplest_ffmpeg_video_encoder\simplest_ffmpeg_video_encoder_pure\simplest_ffmpeg_video_encoder_pure.cpp 编写.
// ffmpeg相关的so是在ubuntu下编译，使用版本：ffmpeg-2.7.7;
// x264使用静态方式编译在 libavcodec-56.so 内.
//

#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
// #include "include/libavfilter/avfilter.h"
// #include "libavformat/avformat.h"
#include "test_kisskyu_mymediatest_h264encoder.h"

// 为了简单，直接用全局变量了
struct AVCodecContext* gpCodecCtx = NULL;
struct AVFrame* gpFrame = NULL;
struct AVPacket gPkt;
// uint8_t* gPicture_buf;

// return 0: successed.
// <0: error code.
JNIEXPORT int JNICALL Java_test_kisskyu_mymediatest_MainActivity_cH264encoderInit
        (JNIEnv * env, jclass obj)
{
    AVCodec *pCodec;
    int  ret;
    int y_size;

    enum AVCodecID codec_id = AV_CODEC_ID_H264;
    int in_w = 540, in_h = 720;

    avcodec_register_all();

    pCodec = avcodec_find_encoder(codec_id);
    if (!pCodec)
    {
        return -1;
    }
    gpCodecCtx = avcodec_alloc_context3(pCodec);
    if (!gpCodecCtx)
    {
        return -2;
    }

    // 参数全部写死先.
    gpCodecCtx->codec_id = codec_id;
    gpCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    gpCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    gpCodecCtx->bit_rate = 1024000;  // bits/s 码率
    gpCodecCtx->gop_size = 50; // I帧间隔
    gpCodecCtx->width = in_w;
    gpCodecCtx->height = in_h;
    gpCodecCtx->me_method = 7;

    gpCodecCtx->time_base.num=1;
    gpCodecCtx->time_base.den=15;  // 帧率

    gpCodecCtx->max_b_frames = 25;  // maximum number of B-frames between non-B-frames

    gpCodecCtx->qmin = 5; // 设定最小质量，与-qmax（设定最大质量）共用，比如-qmin 10 -qmax 31
    gpCodecCtx->qmax = 36; // 数字越小，质量越高。那么码率也可能会超出 bit_rate项.

    if (codec_id == AV_CODEC_ID_H264)
    {
        // 实际发现如果不用baseline，则无论如何会延迟n帧。
        av_opt_set(gpCodecCtx->priv_data, "profile", "baseline", 0); // Baseline profile: 仅支持I帧P帧, 那么max_b_frames无效

        // Current presets in descending order of speed are: ultrafast,superfast, veryfast, faster, fast, medium, slow, slower, veryslow, placebo. The default preset is medium
        av_opt_set(gpCodecCtx->priv_data, "preset", "superfast", 0);
        av_opt_set(gpCodecCtx->priv_data, "tune", "zerolatency", 0); // 零延迟
        // av_opt_set(pCodecCtx->priv_data, "x264opts", "no-mbtree:sliced-threads:sync-lookahead=0", 0);
    }

    if (avcodec_open2(gpCodecCtx, pCodec, NULL) < 0)
    {
        return -3;
    }

    gpFrame = av_frame_alloc();
    if (!gpFrame)
    {
        return -4;
    }
    gpFrame->format = gpCodecCtx->pix_fmt;
    gpFrame->width  = gpCodecCtx->width;
    gpFrame->height = gpCodecCtx->height;

    // int picture_size = avpicture_get_size(gpCodecCtx->pix_fmt, gpCodecCtx->width, gpCodecCtx->height);
    // uint8_t* gPicture_buf = (uint8_t *)av_malloc(picture_size);

    // 在pFrame中填充图像信息.
    // 如果分辨率不变，可以只调用一次该函数，编码每1帧时再直接对gpFrame->data赋值即可.
    //
    avpicture_fill((AVPicture *)gpFrame, NULL, gpCodecCtx->pix_fmt, gpCodecCtx->width, gpCodecCtx->height);

    av_init_packet(&gPkt);
    gPkt.data = NULL;    // packet data will be allocated by the encoder
    gPkt.size = 0;

    return 0;
}

JNIEXPORT void JNICALL Java_test_kisskyu_mymediatest_MainActivity_cH264encoderDestroy
        (JNIEnv * env, jclass obj)
{
    avcodec_close(gpCodecCtx);
    av_free(gpCodecCtx);
    // av_free(gPicture_buf);
    av_frame_free(&gpFrame);

    return;
}

// data: YUV data.
// status output <0: error code,
//                0: ok but no h264 frame output.
//                1: ok and has h264 frame to output.
// return byte[]: Valid if status return 1.
JNIEXPORT jbyteArray JNICALL Java_test_kisskyu_mymediatest_MainActivity_cH264encoderWork
        (JNIEnv * env, jclass obj , jbyteArray data, jint dataLen, jobject status)
{
    // 这段代码是为了像指用c的指针一样，能去修改status变量的值。
    jclass c;
    jfieldID id;
    c = (*env)->FindClass(env, "java/lang/Integer");
    if (c==NULL)
    {
        return 0;
    }
    id = (*env)->GetFieldID(env, c, "value", "I");
    if (id==NULL)
    {
        return 0;
    }
    // End.

    // static int i = 0;
    unsigned char cData[dataLen];
    (*env)->GetByteArrayRegion(env, data, 0, dataLen, cData); // 这会有一次内存拷贝?!
    int y_size = gpCodecCtx->width * gpCodecCtx->height;
    // 这三行也可以使用avpicture_fill函数替代.
    gpFrame->data[0] = cData;                  // Y
    gpFrame->data[1] = cData + y_size;         // U
    gpFrame->data[2] = cData + y_size * 5 / 4; // V

    gpFrame->pts++; //  = i++; // (pCodecCtx->time_base.den) / ((pCodecCtx->time_base.num) * 20);

    int got_output = 0;
    /* encode the image */
    int ret = avcodec_encode_video2(gpCodecCtx, &gPkt, gpFrame, &got_output);
    if (ret < 0)
    {
        (*env)->SetIntField(env, status, id, -1);
        return 0;
    }
    if (got_output)
    {
        // printf("Succeed to encode frame: %5d\tsize:%6d duration:%d, index:%d, %02x%02x%02x%02x\n",
        //        framecnt, pkt.size, pkt.duration, pkt.stream_index, pkt.data[0], pkt.data[1], pkt.data[2], pkt.data[3]);
        jbyteArray array = (*env)->NewByteArray(env, gPkt.size);
        (*env)->SetByteArrayRegion(env, array, 0, gPkt.size, gPkt.data);
        av_free_packet(&gPkt);

        (*env)->SetIntField(env, status, id, 1);
        return array;
    }
    else
    {
        (*env)->SetIntField(env, status, id, 0);
        return 0;
    }
}

