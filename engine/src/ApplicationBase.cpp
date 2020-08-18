#include "ApplicationBase.h"
#include "Renderer.h"
#include "Log.h"
#include "Utils.h"

static Renderer* gRenderer = nullptr;

void ApplicationBase::InitializeRenderer(AppDelegate* appDelegate)
{
	if (gRenderer) {
		delete gRenderer;
		gRenderer = nullptr;
	}
	LOG_INFO("[Application] InitializeRenderer");
	gRenderer = CreateOpenGLRenderer(appDelegate);
}

void ApplicationBase::ReleaseRenderer()
{
	if (gRenderer) {
		delete gRenderer;
		gRenderer = nullptr;
		LOG_INFO("[Application] ReleaseRenderer");
	}
}

void ApplicationBase::MainLoopContent()
{
	if (gRenderer) {
		gRenderer->Render();
	}
}

void ApplicationBase::Resize(int width, int height)
{
	if (gRenderer) {
		gRenderer->Resize(width, height);
	}
}