#pragma once

class AppDelegate;
class ApplicationBase
{
public:
	void InitializeRenderer(AppDelegate* delegate);
	void ReleaseRenderer();
	void MainLoopContent();
	void Resize(int width, int height);
};