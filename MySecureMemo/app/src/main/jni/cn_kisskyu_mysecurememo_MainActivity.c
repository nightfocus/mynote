//
// Created by admin on 2016/11/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cn_kisskyu_mysecurememo_MainActivity.h"

// 密码的字典表，范围是25.
#define DicMatrixCnt 40
const unsigned char DicMatrix[DicMatrixCnt] = {11, 13, 151, 157, 29, 101, 19, 23, 103, 107,
                                               7, 109, 37, 41, 47, 53, 57, 59, 71, 89,
                                               83, 97, 113, 3, 61, 137, 139, 5, 17, 127,
                                               131, 163, 67, 149, 79, 5, 31, 2, 29, 31 };

int pickOne(const char* pd, int s1, int run)
{

    int s=0;
    for(int i=0;i< strlen(pd);i++)
        s = DicMatrix[(s + pd[i]) % DicMatrixCnt] + s1 + run;
    for(int i=0;i<run;i++)
    {
        s = i + s1 + s % DicMatrix[s%DicMatrixCnt];
    }
    return (s%DicMatrixCnt);
}

JNIEXPORT jstring JNICALL Java_cn_kisskyu_mysecurememo_MainActivity_cEncryptPwd
        (JNIEnv * env, jclass jobj, jstring pwd)
{
    const int gepwdlen = 24; // 产生一个24字节的密码
    char *gepwd = (char*)malloc(gepwdlen+1);
    memset(gepwd, 0, gepwdlen+1);

    // 将用户输入的密码转成C字符串
    const char* strpwd = (*env)->GetStringUTFChars(env, pwd, NULL);

    // 根据字典表生成一个24字符长的C字符串gepwd.
    int i;
    int s=0;
    for(i = 0 ; i < gepwdlen; i++)
    {
        s = pickOne(strpwd, s, i+1);
        gepwd[i] = '1'+ s; // 从'1'的ascii码,往后挑一个值
    }

    // 对应 GetStringUTFChars，表示释放内存
    (*env)->ReleaseStringUTFChars(env, pwd, strpwd);
    // 用C字符串gepwd生成一个java的字符串对象jpwd
    jstring jpwd = (*env)->NewStringUTF(env, gepwd);
    free(gepwd);

    return jpwd;
}
