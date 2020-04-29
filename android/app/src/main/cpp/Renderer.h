#pragma once
#include <math.h>

#if __ANDROID_API__ >= 23
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif


// ----------------------------------------------------------------------------
// Types, functions, and data used by both ES2 and ES3 renderers.

// This demo uses three coordinate spaces:
// - The model (a quad) is in a [-1 .. 1]^2 space
// - Scene space is either
//    landscape: [-1 .. 1] x [-1/(2*w/h) .. 1/(2*w/h)]
//    portrait:  [-1/(2*h/w) .. 1/(2*h/w)] x [-1 .. 1]
// - Clip space in OpenGL is [-1 .. 1]^2

// returns true if a GL error occurred
extern bool CheckGlError(const char* funcName);

// ----------------------------------------------------------------------------
// Interface to the renderer implementation, used by JNI code.

class Renderer
{
public:
    virtual ~Renderer();
    void Resize(int w, int h);
    void Render();

protected:
    Renderer();

    virtual void Draw() = 0;

    virtual void Step() = 0;

    uint64_t mLastFrameNs;
    GLfloat _resolution[2];
};

extern Renderer* CreateOpenGLRenderer();