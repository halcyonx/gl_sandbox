#pragma once
#include "ApplicationBase.h"

class Application : public ApplicationBase
{
public:
    void Start(AppDelegate* delegate);
    void Shutdown();
};

void RunApplication(AppDelegate* delegate);
