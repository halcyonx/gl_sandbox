#pragma once
#include "EngineIncludes.h"
#include "AppDelegate.h"

class SandboxApp : public AppDelegate
{
public:
	void Initialize() override;
	void Update(float dt) override;

private:
    GLuint _vbo = 0;
    GLuint _vao = 0;
    GLuint _ebo = 0;
    GLfloat _time = 0.0f;
    Shader _shader;
};
