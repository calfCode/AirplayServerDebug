//
// Created by Administrator on 2019/1/29/029.
//

#ifndef AIRPLAYSERVER_LOG_H
#define AIRPLAYSERVER_LOG_H
#include <android/log.h>

#define TAG "raop"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG ,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG ,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG ,__VA_ARGS__)


#define IJK_LOG_UNKNOWN     ANDROID_LOG_UNKNOWN
#define IJK_LOG_DEFAULT     ANDROID_LOG_DEFAULT

#define IJK_LOG_VERBOSE     ANDROID_LOG_VERBOSE
#define IJK_LOG_DEBUG       ANDROID_LOG_DEBUG
#define IJK_LOG_INFO        ANDROID_LOG_INFO
#define IJK_LOG_WARN        ANDROID_LOG_WARN
#define IJK_LOG_ERROR       ANDROID_LOG_ERROR
#define IJK_LOG_FATAL       ANDROID_LOG_FATAL
#define IJK_LOG_SILENT      ANDROID_LOG_SILENT


#define VLOG(level, TAG, ...)    ((void)__android_log_vprint(level, TAG, __VA_ARGS__))
#define ALOG(level, TAG, ...)    ((void)__android_log_print(level, TAG, __VA_ARGS__))
#define IJK_LOG_TAG "IJKMEDIA"
#define VLOGV(...)  VLOG(IJK_LOG_VERBOSE,   IJK_LOG_TAG, __VA_ARGS__)
#define VLOGD(...)  VLOG(IJK_LOG_DEBUG,     IJK_LOG_TAG, __VA_ARGS__)
#define VLOGI(...)  VLOG(IJK_LOG_INFO,      IJK_LOG_TAG, __VA_ARGS__)
#define VLOGW(...)  VLOG(IJK_LOG_WARN,      IJK_LOG_TAG, __VA_ARGS__)
#define VLOGE(...)  VLOG(IJK_LOG_ERROR,     IJK_LOG_TAG, __VA_ARGS__)

#define ALOGV(...)  ALOG(IJK_LOG_VERBOSE,   IJK_LOG_TAG, __VA_ARGS__)
#define ALOGD(...)  ALOG(IJK_LOG_DEBUG,     IJK_LOG_TAG, __VA_ARGS__)
#define ALOGI(...)  ALOG(IJK_LOG_INFO,      IJK_LOG_TAG, __VA_ARGS__)
#define ALOGW(...)  ALOG(IJK_LOG_WARN,      IJK_LOG_TAG, __VA_ARGS__)
#define ALOGE(...)  ALOG(IJK_LOG_ERROR,     IJK_LOG_TAG, __VA_ARGS__)
#define LOG_ALWAYS_FATAL(...)   do { ALOGE(__VA_ARGS__); exit(1); } while (0)
#endif //AIRPLAYSERVER_LOG_H
