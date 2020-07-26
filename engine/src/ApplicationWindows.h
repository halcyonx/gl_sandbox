#pragma once
#include "ApplicationBase.h"

class AppDelegate;
class Application : public ApplicationBase
{
public:
	void Start(AppDelegate* delegate);
	void Shutdown();
};

void RunApplication(Application& application, AppDelegate* delegate);
