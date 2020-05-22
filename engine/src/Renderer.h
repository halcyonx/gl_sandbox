#pragma once
#include <math.h>
#include <cstdint>
// ----------------------------------------------------------------------------
// Interface to the renderer implementation, used by JNI code.

class Renderer
{
public:
    Renderer() = default;
    virtual ~Renderer() = default;
    virtual void Draw() = 0;
    virtual void Step() = 0;
    void Resize(int w, int h);
    void Render();

    uint32_t mLastFrameNs = 0;
    float _resolution[2];
};

extern Renderer* CreateOpenGLRenderer();