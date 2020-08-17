#include "SandboxApp.h"
#include <Utils.h>

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

void SandboxApp::Initialize()
{	
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	glGenVertexArrays(1, &_vao); glCheckError();
	glGenBuffers(1, &_vbo); glCheckError();
	glGenBuffers(1, &_ebo); glCheckError();

	glBindVertexArray(_vao); glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, _vbo); glCheckError();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); glCheckError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo); glCheckError();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); glCheckError();

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); glCheckError();
	glEnableVertexAttribArray(0); glCheckError();
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); glCheckError();
	glEnableVertexAttribArray(1); glCheckError();
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))); glCheckError();
	glEnableVertexAttribArray(2); glCheckError();

	glBindVertexArray(0); // Unbind VAO
	glCheckError();

	_shader.LoadFromFile("shaders/texture_mix.vs", "shaders/texture_mix.fs");
	_texture_0.Load("textures/crate.png");
	_texture_1.Load("textures/awesomeface.png");
}

void SandboxApp::Update(float dt)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_shader.Bind();
	_shader.BindTexture2D(0, _texture_0.GetHandle());
	_shader.BindTexture2D(1, _texture_1.GetHandle());
	glm::mat4 modelMatrix{ 1.0f };
	modelMatrix = glm::rotate(modelMatrix, utils::getTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	_shader.SetUniform("transform", modelMatrix);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); glCheckError();
	glCheckError();
}

