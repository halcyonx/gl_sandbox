#pragma once

#include "Renderer.h"

class AppDelegate
{
public:
	AppDelegate() = default;
	virtual ~AppDelegate() = default;
	virtual void Initialize() = 0;
	virtual void Update(float dt) = 0;;
	Renderer* renderer;
};