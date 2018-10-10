#ifndef KEYCHECK_INTEGRATE_LOG_H
#define KEYCHECK_INTEGRATE_LOG_H

#include <string.h>
#include <android/log.h>
#include <jni.h>
#include <stdio.h>

#define LOG_TAG    "native-dev"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__) // 定义LOGW类型
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__) // 定义LOGF类型

#endif /* KEYCHECK_INTEGRATE_LOG_H */
