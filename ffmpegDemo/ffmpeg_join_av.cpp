/**
*  这个测试main.cpp是基于ffmpeg3.2.12，
   用于将多个mp4文件（或其它音视频格式）进行截取合并, 例如：
   file1.mp4                                   file2.mp4
   +-------------------------------------------+----------------------------------+
   生成一个新的file3.mp4
             +-------------------------------------------------------+

   编译 ffmpeg:
   ./configure --disable-asm --disable-ffplay --disable-ffserver --disable-ffprobe --enable-shared 
   make clean
   make

   创建 sample2 子目录:
   cp ./libavcodec/libavcodec.so.57 ./libavformat/libavformat.so.57 ./libavutil/libavutil.so.55 ./libswresample/libswresample.so.2 sample2/

   编译这个程序:
   g++ -o main main.cpp -I../ -L../libavformat -lavformat -L../libavcodec/ -lavcodec -L../libavutil/ -lavutil -L../libswresample/ -lswresample

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "../libavformat/avformat.h"

#ifdef __cplusplus
};
#endif


int main(int argc, char* argv[])
{
    AVOutputFormat *ofmt = NULL;
    //Input AVFormatContext and Output AVFormatContext
    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
    AVPacket pkt;
    const char *in_filename, *out_filename, *in_filename2;
    int ret;
    int frame_index = 0;
    double needSkipms = 0;
    AVDictionary* options = NULL;
    if (argc == 1)
    {
        in_filename = "s3.mp4";//Input file 1
        in_filename2 = "s4.mp4"; // Input file 2
        out_filename = "d3.mp4";//Output file URL
        needSkipms = 5000; // 跳过第一个文件的前5秒
    }
    else
    {
        in_filename2 = NULL;
        in_filename = argv[1];
        out_filename = argv[2];
        needSkipms = atoi(argv[3]);
    }

    av_register_all();

    do
    {
        //Input
        if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) 
        {
            printf("Could not open input file %s\n", in_filename);
            break;
        }
        if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) 
        {
            printf("Failed to retrieve input stream information");
            break;
        }
        av_dump_format(ifmt_ctx, 0, in_filename, 0);
        //Output
        avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
        if (!ofmt_ctx) 
        {
            printf("Could not create output context\n");
            ret = AVERROR_UNKNOWN;
            break;
        }
        ofmt = ofmt_ctx->oformat;
        
        for (int i = 0; i < ifmt_ctx->nb_streams; i++)
        {
            //Create output AVStream according to input AVStream
            AVStream *in_stream = ifmt_ctx->streams[i];
            AVStream *out_stream = avformat_new_stream(ofmt_ctx, NULL);
            if (!out_stream)
            {
                printf("Failed allocating output stream\n");
                ret = AVERROR_UNKNOWN;
                break;
            }
            // avcodec_parameters_to_context
            //Copy the settings of AVCodecContext
            if (avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar) < 0)
            {
                printf("Failed to copy context from input to output stream avcodec_parameters_to_context\n");
                break;
            }
            out_stream->codecpar->codec_tag = 0;
            // if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
            //    out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
        }
        /*
        for (int i = 0; i < ifmt_ctx->nb_streams; i++) 
        {
            //Create output AVStream according to input AVStream
            AVStream *in_stream = ifmt_ctx->streams[i];
            AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
            if (!out_stream) 
            {
                printf("Failed allocating output stream\n");
                ret = AVERROR_UNKNOWN;
                break;
            }
            // avcodec_parameters_to_context
            //Copy the settings of AVCodecContext
            if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0) 
            {
                printf("Failed to copy context from input to output stream codec context\n");
                break;
            }
            out_stream->codec->codec_tag = 0;
            // if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
            //    out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
        }
        */
        //Output information------------------
        av_dump_format(ofmt_ctx, 0, out_filename, 1);
        //Open output file
        if (!(ofmt->flags & AVFMT_NOFILE)) 
        {
            ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
            if (ret < 0) 
            {
                printf("Could not open output file '%s'", out_filename);
                break;
            }
        }

        // flag to set the moov atom position to the front.
        av_dict_set(&options, "movflags", "faststart", 0);
        //Write file header
        if (avformat_write_header(ofmt_ctx, &options) < 0) 
        {
            printf("Error occurred when opening output file\n");
            break;
        }

        bool writeFlag = false;
        int64_t vSkipDTS = 0, aSkipDTS = 0;
        int64_t vSkipPTS = 0, aSkipPTS = 0;
        int64_t vWritedDTS = 0, aWritedDTS = 0;
        int64_t vWritedPTS = 0, aWritedPTS = 0;
        int64_t initVideoDTS = 0, initAudioDTS = 0;
        bool firstVideoFrame = true, firstWriteVideoFrame = true;
        bool firstAudioFrame = true, firstWriteAudioFrame = true;
        int isc = 0;
        int aCnt=0, vCnt = 0;
        AVStream *out_stream;
        for(;;)
        {
            AVStream *in_stream;
            //Get an AVPacket
            ret = av_read_frame(ifmt_ctx, &pkt);
            if (ret < 0)
                break;
            in_stream = ifmt_ctx->streams[pkt.stream_index];
            out_stream = ofmt_ctx->streams[pkt.stream_index];

            // printf("-- %d\n", pkt.pts * double(in_stream->time_base.num/in_stream->time_base.den));
            if (!writeFlag)
            {
                if (in_stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
                {
                    if (pkt.flags & AV_PKT_FLAG_KEY)
                    {
                        double tskip = pkt.pts * ((double)in_stream->time_base.num / in_stream->time_base.den) * 1000;
                        if (tskip >= needSkipms)
                        {
                            printf("skip %d video frames\n", isc);
                            writeFlag = true;
                        }
                    }
                    isc++;

                    if (firstVideoFrame)
                    {
                        firstVideoFrame = false;
                        initVideoDTS = pkt.dts; // 如果有B帧存在，第一个dts的值，可能小于0
                    }
                    // printf("I-Frame  VIDEO  pkt.pts:%ld, dts:%ld, duration:%ld\n", pkt.pts, pkt.dts, pkt.duration);
                }
                else
                {
                    if (firstAudioFrame)
                    {
                        firstAudioFrame = false;
                        initAudioDTS = pkt.dts;
                    }
                }
            }

            if (!writeFlag)
            {
                av_packet_unref(&pkt);
                continue;
            }
            else
            {
                // printf("prepare to write.\n");
                if (in_stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
                {
                    if (firstWriteVideoFrame)
                    {
                        vSkipPTS = pkt.pts;
                        vSkipDTS = pkt.dts - initVideoDTS;

                        pkt.pts -= vSkipPTS; // usually still equal 0
                        pkt.dts -= vSkipDTS;
                        firstWriteVideoFrame = false;
                    }
                    else
                    {
                        pkt.pts -= vSkipPTS;
                        pkt.dts -= vSkipDTS;
                    }
                    vCnt++;
                    printf("V pkt.pts:%ld, dts:%ld, duration:%ld\n", pkt.pts, pkt.dts, pkt.duration);
                    vWritedDTS = pkt.dts;
                    vWritedPTS = pkt.pts;
                }
                else
                {
                    if (firstWriteAudioFrame)
                    {
                        aSkipPTS = pkt.pts;
                        aSkipDTS = pkt.dts - initAudioDTS;
                        pkt.pts -= aSkipPTS;// usually still equal 0
                        pkt.dts -= aSkipDTS;
                        firstWriteAudioFrame = false;
                    }
                    else
                    {
                        pkt.pts -= aSkipPTS;
                        pkt.dts -= aSkipDTS;
                    }
                    aCnt++;
                    printf("AUDIO pkt.pts:%ld, dts:%ld, duration:%ld\n", pkt.pts, pkt.dts, pkt.duration);
                    aWritedDTS = pkt.dts;
                    aWritedPTS = pkt.pts;
                }
            }
            // continue;

            // Convert PTS/DTS
            pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
            pkt.pos = -1;
            // printf("AFTER   pkt.pts:%ld, dts:%ld, duration:%ld\n", pkt.pts, pkt.dts, pkt.duration);
            //Write
            int rb = 0;
            if ((rb = av_interleaved_write_frame(ofmt_ctx, &pkt)) < 0) 
            {
                printf("Error muxing packet, %d\n", rb);
                break;
            }

            // printf("Write %d/%d, %lld, duration:%d,  %8d frames to output file\n", in_stream->time_base.num, in_stream->time_base.den, pkt.dts, pkt.duration, frame_index);
            av_packet_unref(&pkt);
            frame_index++;
        }

        printf("Output audio %d frames, video %d frames\n\n", aCnt, vCnt);


        //*
        // 第一个文件写完，追加第二个进行合成
        // 假定第二个文件的格式/编码完全等同.
        if (in_filename2 != NULL)
        {
            AVFormatContext *ifmt_ctx2 = NULL;
            //Input2
            if ((ret = avformat_open_input(&ifmt_ctx2, in_filename2, 0, 0)) < 0)
            {
                printf("Could not open input file %s\n", in_filename2);
            }
            if ((ret = avformat_find_stream_info(ifmt_ctx2, 0)) < 0)
            {
                printf("Failed to retrieve input stream information");
            }

            if (ret >= 0)
            {
                av_dump_format(ifmt_ctx2, 0, in_filename2, 0);
                for (;;)
                {
                    AVStream *in_stream2;
                    //Get an AVPacket
                    ret = av_read_frame(ifmt_ctx2, &pkt);
                    if (ret < 0)
                        break;
                    in_stream2 = ifmt_ctx2->streams[pkt.stream_index];
                    out_stream = ofmt_ctx->streams[pkt.stream_index];

                    // 第二个文件的pkt，里面的dts,pts 需要按已写入的累加后，再进行convert.
                    if (in_stream2->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
                    {
                        vWritedPTS += pkt.duration;
                        vWritedDTS += pkt.duration;
 
                        pkt.pts = vWritedPTS;
                        pkt.dts = vWritedDTS;
                        printf("----------------V   pkt.pts:%ld, dts:%ld, duration:%ld\n", pkt.pts, pkt.dts, pkt.duration);
                    }
                    else
                    {
                        aWritedPTS += pkt.duration;
                        aWritedDTS += pkt.duration;

                        pkt.pts = aWritedPTS;
                        pkt.dts = aWritedPTS;
                        printf("----------------AUDIO   pkt.pts:%ld, dts:%ld, duration:%ld\n", pkt.pts, pkt.dts, pkt.duration);
                    }
                    
                    
                    // Convert PTS/DTS
                    pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream2->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
                    pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream2->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
                    pkt.duration = av_rescale_q(pkt.duration, in_stream2->time_base, out_stream->time_base);
                    pkt.pos = -1;

                    //Write
                    int rb = 0;
                    if ((rb = av_interleaved_write_frame(ofmt_ctx, &pkt)) < 0)
                    {
                        printf("Error muxing packet at in_filename2, %d\n", rb);
                        break;
                    }
                    
                    // printf("Write %d/%d, %lld, duration:%d,  %8d frames to output file\n", in_stream->time_base.num, in_stream->time_base.den, pkt.dts, pkt.duration, frame_index);
                    av_packet_unref(&pkt);
                    frame_index++;
                }
            }

            avformat_close_input(&ifmt_ctx2);
        }
        //*/

        //Write file trailer
        av_write_trailer(ofmt_ctx);

    }while (false);

    avformat_close_input(&ifmt_ctx);
    /* close output */
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);


    while (1)
        sleep(1);
    return 0;
}
