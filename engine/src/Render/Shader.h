#pragma once

#include <string>

class Shader
{
public:
    void LoadFromFile(const std::string vsPath, const std::string fsPath);
    void Bind();
private:
    uint32_t _program;
};
