#pragma once

#ifdef PLATFORM_ANDROID
#include <android/log.h>

#define DEBUG 1

#define LOG_TAG "GL_SANDBOX"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

#define LOG_INFO(...)		  ALOGV(__VA_ARGS__)
#define LOG_ERROR(...)		  ALOGE(__VA_ARGS__)

#else
#define ALOGV(...)

#endif
#elif PLATFORM_WINDOWS
#define LOG_INFO(...)
#define LOG_ERROR(...)
#endif

