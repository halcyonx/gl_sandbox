#include "Shader.h"
#include <android/log.h>

#if __ANDROID_API__ >= 23
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif

#include <regex>
#include <fstream>
#include <sstream>

#define DEBUG 1

#define LOG_TAG "GL_TESTED"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

namespace Render
{
	using ShaderSources = std::vector<std::pair<GLenum, std::string>>;

	static GLenum ShaderTypeFromString(const std::string & type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		ALOGV("Unknown shader type");
		return 0;
	}

	ShaderSources ParseShaderFile(const std::string &shaderSource)
	{
		ShaderSources sources;
		sources.reserve(2);

		const std::string token("#type");
		const std::regex rgx(R"(^#type\s+(\w+)\s*$)");
		size_t pos = shaderSource.find(token);

		while (pos != std::string::npos)
		{
			size_t eol = shaderSource.find_first_of('\n', pos);
			const std::string line = shaderSource.substr(pos, eol - pos);
			std::smatch match;
			
			if (std::regex_search(line.begin(), line.end(), match, rgx))
			{
				std::string typeStr = match[1];
				GLenum shaderType = ShaderTypeFromString(typeStr);

				eol++;
				pos = shaderSource.find(token, eol);
				std::string src = (pos == std::string::npos ? shaderSource.substr(eol) : shaderSource.substr(eol, pos - eol));
				sources.emplace_back(shaderType, src);
			}
		}

		return sources;
	}

	void Shader::Compile(const std::string & vertexSource, const std::string & fragmentSource)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar *source = (const GLchar *)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::string infoLog(maxLength, 0);

			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, std::data(infoLog));
			glDeleteShader(vertexShader);

			ALOGV("Vertex shader compilation failed: %s", infoLog.c_str());
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = (const GLchar *)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::string infoLog(maxLength, 0);

			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, std::data(infoLog));
			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			ALOGV("Fragment shader compilation failed: %s", infoLog.c_str());
			return;
		}

		uint32_t program = glCreateProgram();

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::string infoLog;
			infoLog.reserve(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteProgram(program);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			ALOGV("Shader program linkage failed: %s", infoLog.c_str());

			return;
		}

		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		_program = program;
	}

	void Shader::LoadFromSource(const std::string & shaderPath)
	{
		if (!FileExists(shaderPath))
		{
			ALOGE("File: [%s] does not exists", shaderPath.c_str());
			return;
		}

		std::ifstream shaderFile;
		shaderFile.open(shaderPath);

		std::stringstream fileStream;
		fileStream << shaderFile.rdbuf();
		shaderFile.close();

		ShaderSources sources = ParseShaderFile(fileStream.str());

		if (sources.size() == 2)
		{
			Compile(sources[0].second, sources[1].second);
		}
		else
		{
			ALOGV("Incorrect shader sources count");
		}
	}

	void Shader::LoadFromString(const std::string & vertexSource, const std::string & fragmentSource)
	{
		Compile(vertexSource, fragmentSource);
	}

	void Shader::Bind() const
	{
		glUseProgram(_program);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform(const std::string & uniformName, int value) const
	{
		glUniform1i(glGetUniformLocation(_program, uniformName.c_str()), value);
	}

	void Shader::SetUniform(const std::string & uniformName, float value) const
	{
		glUniform1f(glGetUniformLocation(_program, uniformName.c_str()), value);
	}

	void Shader::SetUniform(const std::string & uniformName, const Math::Vec3& vector) const
	{
		glUniform3fv(glGetUniformLocation(_program, uniformName.c_str()), 1, &vector[0]);
	}

	void Shader::SetUniform(const std::string & uniformName, const Math::Vec4& vector) const
	{
		glUniform4fv(glGetUniformLocation(_program, uniformName.c_str()), 1, &vector[0]);
	}

    void Shader::SetUniform(const std::string &uniformName, const Math::Vec2 &vector) const
    {
        glUniform2fv(glGetUniformLocation(_program, uniformName.c_str()), 1, &vector[0]);
    }

	bool Shader::FileExists(const std::string &path) const {
		std::ifstream fs(path);
		return fs.is_open();
	}

//	void Shader::SetUniform(const std::string & uniformName, const mat4 & matrix) const
//	{
//		glUniformMatrix4fv(glGetUniformLocation(_program, uniformName.c_str()), 1, GL_FALSE, &matrix[0][0]);
//	}
}
