#include "Renderer.h"
#include "Log.h"
#include "AndroidUtils.h"
#include "Render/Shader.h"
#include <EGL/egl.h>
#include <fstream>
#include <sstream>
#include <clocale>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

GLfloat vertices[] = {
        // positions          // colors           // texture coords
        0.8f,  0.8f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.8f, -0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.8f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.8f,  0.8f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
};


class RendererOpenGL: public Renderer {
public:
    RendererOpenGL();
    virtual ~RendererOpenGL();
    bool Initialize();

protected:
    virtual void Draw();
    virtual void Step() override;

private:

    const EGLContext _eglContext;

    GLuint _vbo = 0;
    GLuint _vao = 0;
    GLuint _ebo = 0;
    GLfloat _time = 0.0f;
    Shader _shader;
};

Renderer* CreateOpenGLRenderer()
{
    RendererOpenGL* renderer = new RendererOpenGL;
    if (!renderer->Initialize())
    {
        delete renderer;
        return nullptr;
    }
    return renderer;
}

RendererOpenGL::RendererOpenGL()
    : _eglContext(eglGetCurrentContext())
{
}

bool RendererOpenGL::Initialize()
{
    LOG_INFO("RendererOpenGL::Initialize()");
    LOG_INFO("Using OpenGL ES 3.0 renderer");

    //_program = createProgram(vs.c_str(), fs.c_str());
    _shader.LoadFromFile("shaders/simple.vs", "shaders/simple.fs");

    ///
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    return true;
}

RendererOpenGL::~RendererOpenGL()
{
    /* The destructor may be called after the context has already been
     * destroyed, in which case our objects have already been destroyed.
     *
     * If the context exists, it must be current. This only happens when we're
     * cleaning up after a failed init().
     */
    if (eglGetCurrentContext() != _eglContext)
    {
        return;
    }
}

void RendererOpenGL::Draw()
{
    _shader.Bind();
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void RendererOpenGL::Step() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    auto nowNs = now.tv_sec*1000000000ull + now.tv_nsec;

    if (mLastFrameNs > 0)
    {
        float dt = float(nowNs - mLastFrameNs) * 0.00000001f;
        _time += 0.01f;
    }

    mLastFrameNs = nowNs;
}

