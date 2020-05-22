#include "Shader.h"
#include "AndroidUtils.h"
#include <Log.h>
#include "Renderer.h"
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <exception>


bool checkGlError(const char* funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}


GLuint createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                ALOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
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


std::string GetFileContent(const std::string& path)
{
    std::string str;
    if (FILE* file = android_fopen(path.c_str(), "rb")) {
        char *buffer = nullptr;
        // obtain file size:
        fseek (file , 0 , SEEK_END);
        long pos = ftell(file);
        rewind(file);

        auto nSize = static_cast<size_t>(pos);
        // allocate memory to contain the whole file:
        buffer = (char*) malloc (sizeof(char)*nSize + 1);

        size_t result = fread(buffer, 1, nSize, file);
        if (result == nSize) {
            buffer[nSize] = 0;
            ALOGV("File successfully read: %s", buffer);
        } else {
            ALOGE("File reading failed");
        }

        str = std::string(buffer);

        fclose(file);
    }
    else {
        ALOGE("Failed to open: %s", path.c_str());
    }
    return str;
}

void Shader::LoadFromFile(const std::string vsPath, const std::string fsPath) {
    const std::string& vertexSrc = GetFileContent(vsPath);
    const std::string& fragmentSrc = GetFileContent(fsPath);
    ALOGV("vertexSrc: %s\n\n", vertexSrc.c_str());
    ALOGV("fragmentSrc: %s\n\n", fragmentSrc.c_str());
    _program = createProgram(vertexSrc.c_str(), fragmentSrc.c_str());
}

void Shader::Bind() {
    glUseProgram(_program);
}
