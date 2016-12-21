# 基于ffmpeg-2.7.7 和 libx264 (x264-snapshot-20161219-2245) 编译生成：

LOCAL_PATH := $(call my-dir)

# FFmpeg library
include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := libavcodec-56.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := libavformat-56.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := libavutil-54.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := swresample
LOCAL_SRC_FILES := libswresample-1.so
include $(PREBUILT_SHARED_LIBRARY)


# Program
include $(CLEAR_VARS)
LOCAL_MODULE := h264encoder
LOCAL_SRC_FILES := test_kisskyu_mymediatest_h264encoder.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_LDLIBS :=-llog -lz
LOCAL_SHARED_LIBRARIES := avcodec avformat avutil swresample
include $(BUILD_SHARED_LIBRARY)