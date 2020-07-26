#ifdef PLATFORM_ANDROID
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iosfwd>

#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"

#include "AndroidUtils.h"
#include "ApplicationAndroid.h"
#include "Renderer.h"
#include "Log.h"
#include "Utils.h"
#include "glutils.h"


static Application* application = nullptr;
static AppDelegate* appDelegate = nullptr;
static jobject androidJavaAssetManager = nullptr;

void RunApplication(AppDelegate* delegate)
{
    appDelegate = delegate;
}

void Application::Start(AppDelegate* delegate)
{
    InitializeRenderer(delegate);
    LOG_INFO("Application started");
}

void Application::Shutdown()
{
    LOG_INFO("Application shutdown");
}

extern "C"
{
    JNIEXPORT void JNICALL Java_com_android_glrenderer_GameJNILib_Initialize(JNIEnv *env, jclass clazz,
                                                                             jobject assetManager,
                                                                             jstring apkPath);
    JNIEXPORT void JNICALL Java_com_android_glrenderer_GameJNILib_Resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_glrenderer_GameJNILib_Render(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_glrenderer_GameJNILib_Shutdown(JNIEnv* env, jobject obj);
};

JNIEXPORT void JNICALL
Java_com_android_glrenderer_GameJNILib_Initialize(JNIEnv *env, jclass clazz, jobject assetManager,
                                                  jstring apkPath)
{
    if (const char *utf8 = (*env).GetStringUTFChars(apkPath, nullptr)) {
        LOG_INFO("ApkPath: %s", utf8);
    }

    androidJavaAssetManager = (*env).NewGlobalRef(assetManager);
    android_fopen_set_asset_manager(AAssetManager_fromJava(env, androidJavaAssetManager));
    LOG_INFO("GameJNILib_Initialize");
    if (application) {
        delete application;
        application = nullptr;
    }
    application = new Application;
    application->Start(appDelegate);
}

JNIEXPORT void JNICALL
Java_com_android_glrenderer_GameJNILib_Resize(JNIEnv* env, jobject obj, jint width, jint height)
{
    if (application) {
        application->Resize(width, height);
        LOG_INFO("Application resized: %d %d", width, height);
    }
}

JNIEXPORT void JNICALL
Java_com_android_glrenderer_GameJNILib_Render(JNIEnv* env, jobject obj)
{
    if (application) {
        application->MainLoopContent();
    }
}

JNIEXPORT void JNICALL
Java_com_android_glrenderer_GameJNILib_Shutdown(JNIEnv* env, jobject obj)
{
    if (application) {
        application->Shutdown();
        delete application;
    }
    if (androidJavaAssetManager) {
        (*env).DeleteGlobalRef(androidJavaAssetManager);
        androidJavaAssetManager = nullptr;
        android_fopen_set_asset_manager(nullptr);
    }
}
#endif
