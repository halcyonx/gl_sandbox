#pragma once

class ApplicationBase
{
public:
    void InitializeRenderer();
    void MainLoopContent();
    void Resize(int width, int height);
};