#ifndef ANDROID_FOPEN_H
#define ANDROID_FOPEN_H
#ifdef PLATFORM_ANDROID
#include <stdio.h>
#include <android/asset_manager.h>

#ifdef __cplusplus
extern "C" {
#endif

void android_fopen_set_asset_manager(AAssetManager* manager);
FILE* android_fopen(const char* fname, const char* mode);

#define fopen(name, mode) android_fopen(name, mode)

#ifdef __cplusplus
}
#endif
#endif
#endif