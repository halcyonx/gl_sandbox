#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iosfwd>

#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"

#include "AndroidUtils.h"
#include "Renderer.h"
#include "Log.h"

bool CheckGlError(const char* funcName)
{
    GLint err = glGetError();
    if (err != GL_NO_ERROR)
    {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

static void printGlString(const char* name, GLenum s)
{
    const char* v = (const char*)glGetString(s);
    ALOGV("GL %s: %s\n", name, v);
}

// ----------------------------------------------------------------------------

Renderer::Renderer()
    : mLastFrameNs(0)
{
}

Renderer::~Renderer()
{
}

void Renderer::Resize(int w, int h)
{
    _resolution[0] = w;
    _resolution[1] = h;
    glViewport(0, 0, w, h);
}

void Renderer::Render()
{
    Step();

    glClearColor(0.3f, 0.1f, 0.67f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Draw();
    CheckGlError("Renderer::Render");
}

// ----------------------------------------------------------------------------

static Renderer* gRenderer = nullptr;
static jobject androidJavaAssetManager = nullptr;

extern "C"
{
    JNIEXPORT void JNICALL Java_com_android_glrenderer_GameJNILib_Init(JNIEnv *env, jclass clazz,
                                                                       jobject assetManager,
                                                                       jstring apkPath);
    JNIEXPORT void JNICALL Java_com_android_glrenderer_GameJNILib_Resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_glrenderer_GameJNILib_Render(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_glrenderer_GameJNILib_Shutdown(JNIEnv* env, jobject obj);
};

JNIEXPORT void JNICALL
Java_com_android_glrenderer_GameJNILib_Init(JNIEnv *env, jclass clazz, jobject assetManager,
                                            jstring apkPath)
{
    if (gRenderer) {
        delete gRenderer;
        gRenderer = nullptr;
    }

    if (const char *utf8 = (*env).GetStringUTFChars(apkPath, nullptr)) {
        ALOGV("ApkPath: %s", utf8);
    }


    androidJavaAssetManager = (*env).NewGlobalRef(assetManager);
    android_fopen_set_asset_manager(AAssetManager_fromJava(env, androidJavaAssetManager));
    ALOGV("[here3] Java_com_android_glrenderer_GameJNILib_Init");

    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.") ) {
        gRenderer = CreateOpenGLRenderer();
    } else {
        ALOGE("Unsupported OpenGL ES version");
    }
}

JNIEXPORT void JNICALL
Java_com_android_glrenderer_GameJNILib_Resize(JNIEnv* env, jobject obj, jint width, jint height)
{
    if (gRenderer) {
        gRenderer->Resize(width, height);
    }
}

JNIEXPORT void JNICALL
Java_com_android_glrenderer_GameJNILib_Render(JNIEnv* env, jobject obj)
{
    if (gRenderer) {
        gRenderer->Render();
    }
}

JNIEXPORT void JNICALL
Java_com_android_glrenderer_GameJNILib_Shutdown(JNIEnv* env, jobject obj)
{
    ALOGV("[here3] Java_com_android_glrenderer_GameJNILib_Shutdown");
    if (androidJavaAssetManager) {
        (*env).DeleteGlobalRef(androidJavaAssetManager);
        androidJavaAssetManager = nullptr;
        android_fopen_set_asset_manager(nullptr);
    }
}
