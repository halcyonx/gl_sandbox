#include "RendererOpenGL.h"
#include "Log.h"
#include "AndroidUtils.h"
#include "Render/Shader.h"
#include <clocale>
#include <time.h>
#include "glutils.h"
#include "Utils.h"
#include "AppDelegate.h"


static void PrintGlString(const char* name, GLenum s)
{
    const char* v = (const char*)glGetString(s);
    LOG_INFO("GL %s: %s\n", name, v);
}


class RendererOpenGL : public Renderer {
public:
    RendererOpenGL();
    virtual ~RendererOpenGL();
    bool Initialize(AppDelegate* appDelegate);

protected:
    virtual void Draw() override;
    virtual void Step() override;
    virtual void Resize(int w, int h) override;
    virtual void Render() override;

private:
#ifdef PLATFORM_ANDROID
    const EGLContext _eglContext;
#endif
    AppDelegate* _appDelegate = nullptr;
    uint32_t mLastFrameNs = 0;
    float _time = 0.0f;
};

Renderer* CreateOpenGLRenderer(AppDelegate* appDelegate)
{
    RendererOpenGL* renderer = nullptr;
    if (const char* versionStr = (const char*)glGetString(GL_VERSION)) {
        renderer = new RendererOpenGL;

        if (!renderer->Initialize(appDelegate)) {
            delete renderer;
            return nullptr;
        }
        PrintGlString("Version", GL_VERSION);
        PrintGlString("Vendor", GL_VENDOR);
        PrintGlString("Renderer", GL_RENDERER);
        PrintGlString("Extensions", GL_EXTENSIONS);
    }
    else {
        LOG_ERROR("Unsupported OpenGL ES version");
    }

    return renderer;
}

#ifdef PLATFORM_WINDOWS
RendererOpenGL::RendererOpenGL()
{
}
#elif PLATFORM_ANDROID
RendererOpenGL::RendererOpenGL()
    : _eglContext(eglGetCurrentContext())
{
}
#endif

bool RendererOpenGL::Initialize(AppDelegate* appDelegate)
{
	if (appDelegate)
	{
        _appDelegate = appDelegate;
        _appDelegate->Initialize();
	}
    LOG_INFO("RendererOpenGL::Initialize()");
    LOG_INFO("Using OpenGL ES 3.0 renderer");

    return true;
}

RendererOpenGL::~RendererOpenGL()
{
    /* The destructor may be called after the context has already been
     * destroyed, in which case our objects have already been destroyed.
     *
     * If the context exists, it must be current. This only happens when we're
     * cleaning up after a failed init().
     */
#ifdef PLATFORM_ANDROID
    if (eglGetCurrentContext() != _eglContext)
    {
        return;
    }
#endif
}

void RendererOpenGL::Draw()
{
    
}

void RendererOpenGL::Step() {
//    timespec now;
//    clock_gettime(CLOCK_MONOTONIC, &now);
//    auto nowNs = now.tv_sec * 1000000000ull + now.tv_nsec;
//
//    if (mLastFrameNs > 0)
//    {
//         float dt = float(nowNs - mLastFrameNs) * 0.00000001f;
//         _time += dt;
//    }
//
//    mLastFrameNs = nowNs;
}

void RendererOpenGL::Resize(int w, int h) {
    _resolution[0] = w;
    _resolution[1] = h;
    glViewport(0, 0, w, h);
}

void RendererOpenGL::Render() {
    Step();
    _appDelegate->Update(1.0f);
}
