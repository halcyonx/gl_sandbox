#pragma once
#ifdef PLATFORM_ANDROID
#include <EGL/egl.h>
#if __ANDROID_API__ >= 23
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif
#elif PLATFORM_WINDOWS
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#endif
