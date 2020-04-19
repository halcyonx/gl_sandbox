#pragma once

#include "Math/Vector.h"
#include <string>

namespace Render
{
	class Shader
	{
		friend class Shader;
	public:
		void LoadFromString(const std::string& vertexSource, const std::string& fragmentSource);
		void LoadFromSource(const std::string& shaderPath);
		void Bind() const;
		void Unbind() const;

		void SetUniform(const std::string& uniformName, int value) const;
		void SetUniform(const std::string& uniformName, float value) const;
        void SetUniform(const std::string& uniformName, const Math::Vec2& vector) const;
		void SetUniform(const std::string& uniformName, const Math::Vec3& vector) const;
		void SetUniform(const std::string& uniformName, const Math::Vec4& vector) const;
		// void SetUniform(const std::string& uniformName, const mat4& matrix) const;

	protected:
		void Compile(const std::string & vertexSource, const std::string & fragmentSource);
		bool FileExists(const std::string& path) const;

	private:
		uint32_t _program = 0;
	};
}
