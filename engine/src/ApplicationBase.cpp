#include "ApplicationBase.h"
#include "Renderer.h"
#include "Log.h"
#include "Utils.h"

static Renderer* gRenderer = nullptr;

void ApplicationBase::InitializeRenderer(AppDelegate* appDelegate)
{
	LOG_INFO("[Application] InitializeRenderer");
	if (!gRenderer) {
		gRenderer = CreateOpenGLRenderer(appDelegate);
	}
	else {
		LOG_INFO("[Application] InitializeRenderer already initialized");
	}
}

void ApplicationBase::ReleaseRenderer()
{
	if (gRenderer) {
		LOG_INFO("[Application] ReleaseRenderer");
		delete gRenderer;
		gRenderer = nullptr;
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