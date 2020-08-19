#pragma once
#include "EngineIncludes.h"
#include "AppDelegate.h"

class SandboxApp : public AppDelegate
{
public:
	~SandboxApp() override;
	void Initialize() override;
	void Update(float dt) override;

private:
	GLuint _vbo, _vao, _vboBox, _vaoBox, _ebo;
	Shader _shader, _textured;
	Core::Texture _texture_0, _texture_1, _texture_2;
};
