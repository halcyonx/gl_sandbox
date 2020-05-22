#pragma once

#include <string>

class Shader
{
public:
    void LoadFromFile(const std::string& vsPath, const std::string& fsPath);
    void Bind() const;
    void Unbind() const;

private:
    uint32_t Compile(const std::string& vertexSource, const std::string& fragmentSource);

private:
    uint32_t _program;
};
