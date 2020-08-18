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
    virtual float Step() = 0;
    virtual void Resize(int width, int height) = 0;
    virtual void Render() = 0;
    int GetWidth() const;
    int GetHeight() const;
    float GetAspectRatio() const;

protected:
    float _resolution[2];
};

extern Renderer* CreateOpenGLRenderer(AppDelegate* appDelegate);
