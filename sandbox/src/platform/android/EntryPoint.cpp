#include <jni.h>
#include <Log.h>
#include <application/SandboxApp.h>

extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    ALOGV("JNI_OnLoad executed");

    JNIEnv* env;
    if(vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        ALOGV("Initial Get env error");
        return JNI_ERR;
    }

    RunApplication(new SandboxApp);

	return JNI_VERSION_1_6;
}

