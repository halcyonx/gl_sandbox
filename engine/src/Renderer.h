#pragma once
#include <cstdint>

class AppDelegate;

// ----------------------------------------------------------------------------
// Interface to the renderer implementation, used by JNI code.

class Renderer
{
public:
    Renderer() = default;
    virtual ~Renderer() = default;
    virtual void Draw() = 0;
    virtual void Step() = 0;
    virtual void Resize(int width, int height) = 0;
    virtual void Render() = 0;

protected:
    float _resolution[2];
};

extern Renderer* CreateOpenGLRenderer(AppDelegate* appDelegate);
