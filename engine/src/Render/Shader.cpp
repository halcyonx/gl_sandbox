#include "Shader.h"
#include "AndroidUtils.h"
#include "Renderer.h"
#include "Log.h"
#include "Utils.h"
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <exception>
#include "glutils.h"

GLuint createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        GL_CHECK_ERRORS
        return 0;
    }
    glShaderSource(shader, 1, &src, nullptr);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = new GLchar[infoLogLen];
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, nullptr, infoLog);
                LOG_ERROR("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                delete [] infoLog;
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void Shader::LoadFromFile(const std::string& vsPath, const std::string& fsPath) {
    const std::string vertexSrc = utils::GetFileContent(vsPath);
    const std::string fragmentSrc = utils::GetFileContent(fsPath);
    _program = Compile(vertexSrc.c_str(), fragmentSrc.c_str());
}

void Shader::Bind() const {
    glUseProgram(_program);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

uint32_t Shader::Compile(const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vertexSource.c_str());
    if (!vtxShader) {
        goto exit;
    }

    fragShader = createShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());
    if (!fragShader) {
        goto exit;
    }

    program = glCreateProgram();
    if (!program) {
        GL_CHECK_ERRORS
        goto exit;
    }

    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked) {
        LOG_ERROR("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);

        if (infoLogLen) {
            GLchar* infoLog = new GLchar[infoLogLen];

            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, nullptr, infoLog);
                LOG_ERROR("Could not link program:\n%s\n", infoLog);
                delete [] infoLog;
            }
        }

        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}


