#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    void LoadFromFile(const std::string& vsPath, const std::string& fsPath);
    void Bind() const;
    void Unbind() const;
	
	void SetUniform(const std::string_view uniformName, int value) const;
	void SetUniform(const std::string_view uniformName, float value) const;
	void SetUniform(const std::string_view uniformName, const glm::vec3& vector) const;
	void SetUniform(const std::string_view uniformName, const glm::vec4& vector) const;
	void SetUniform(const std::string_view uniformName, const glm::mat4& matrix) const;
	void SetSampler(const std::string_view samplerName, int sampler) const;

private:
    uint32_t Compile(const std::string& vertexSource, const std::string& fragmentSource);

private:
    uint32_t _program;
};
