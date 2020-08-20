#include "Log.h"
#include "AndroidUtils.h"
#include "Render/Shader.h"
#include <clocale>
#include <chrono>
#include <time.h>
#include <Renderer.h>

#include "glutils.h"
#include "Utils.h"
#include "AppDelegate.h"
#include "Timer.h"

#if PLATFORM_WINDOWS
#include <iostream>
#endif

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
	virtual float Step() override;
	virtual void Resize(int w, int h) override;
	virtual void Render() override;

private:
#ifdef PLATFORM_ANDROID
	const EGLContext _eglContext;
#endif
	AppDelegate* _appDelegate = nullptr;
	float _lastTime = 0.0f;
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
		appDelegate->renderer = renderer;
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
	delete _appDelegate;
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

float RendererOpenGL::Step()
{
	const float curTime = utils::GetTime();
	const float dt = curTime - _lastTime;
	_lastTime = curTime;
	return dt;
}

void RendererOpenGL::Resize(int w, int h) {
	_resolution[0] = w;
	_resolution[1] = h;
	_aspect = _resolution[0] / _resolution[1];
	glViewport(0, 0, w, h);
	LOG_INFO("Resolutuion: %d x %d, aspect: %0.3f", GetWidth(), GetHeight(), GetAspectRatio());
}

void RendererOpenGL::Render()
{
	const float dt = Step();
	_appDelegate->Update(dt);
}

float Renderer::GetAspectRatio() const {
	return _aspect;
}

int Renderer::GetHeight() const {
	return _resolution[1];
}

int Renderer::GetWidth() const {
	return _resolution[0];
}
