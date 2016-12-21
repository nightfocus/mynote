//
// Created by admin on 2016/12/21.
//

#include "include/libavcodec/avcodec.h"
// #include "include/libavfilter/avfilter.h"
#include "include/libavformat/avformat.h"
#include "test_kisskyu_mymediatest_h264encoder.h"

JNIEXPORT jstring JNICALL Java_test_kisskyu_mymediatest_MainActivity_cH264encoderInit
        (JNIEnv * env, jclass obj)
{
    char info[40000]={0};
    av_register_all();

    struct URLProtocol *pup = NULL;
    //Input
    struct URLProtocol **p_temp = &pup;
    avio_enum_protocols((void **)p_temp, 0);
    while ((*p_temp) != NULL){
        sprintf(info, "%s[In ][%10s]\n", info, avio_enum_protocols((void **)p_temp, 0));
    }
    pup = NULL;
    //Output
    avio_enum_protocols((void **)p_temp, 1);
    while ((*p_temp) != NULL){
        sprintf(info, "%s[Out][%10s]\n", info, avio_enum_protocols((void **)p_temp, 1));
    }

    return (*env)->NewStringUTF(env, info);
}
