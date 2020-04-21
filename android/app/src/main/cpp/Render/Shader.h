#pragma once

#include <string_view>
#include "Renderer.h"

class Shader
{
public:
    void LoadFromFile(const std::string_view vsPath, const std::string_view fsPath);
    void Bind();
private:
    GLuint _program;
};
