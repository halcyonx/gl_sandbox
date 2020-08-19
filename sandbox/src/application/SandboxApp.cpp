#include "SandboxApp.h"
#include "geometry.h"

namespace
{
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Texture Coords
		1.0f,  0.0f, -1.0f,   16.0f, 16.0f, // Top Right
		1.0f,  0.0f,  1.0f,   16.0f, 0.0f, // Bottom Right
		-1.0f, 0.0f,  1.0f,   0.0f, 0.0f, // Bottom Left
		-1.0f, 0.0f, -1.0f,   0.0f, 16.0f  // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
}

SandboxApp::~SandboxApp()
{
	glDeleteVertexArrays(1, &_vaoBox); glCheckError();
	glDeleteVertexArrays(1, &_vao); glCheckError();
	glDeleteBuffers(1, &_vboBox); glCheckError();
	glDeleteBuffers(1, &_vbo); glCheckError();
	glDeleteBuffers(1, &_ebo); glCheckError();
	LOG_INFO("SandboxApp destroyed");
}

void SandboxApp::Initialize()
{
	LOG_INFO("SandboxApp initialized");
	glEnable(GL_DEPTH_TEST);
	glCheckError();
	glDisable(GL_BLEND);
	glCheckError();

	{
		glGenVertexArrays(1, &_vao); glCheckError();
		glGenBuffers(1, &_vbo); glCheckError();
		glGenBuffers(1, &_ebo); glCheckError();

		glBindVertexArray(_vao); glCheckError();

		glBindBuffer(GL_ARRAY_BUFFER, _vbo); glCheckError();
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); glCheckError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo); glCheckError();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); glCheckError();

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); glCheckError();
		glEnableVertexAttribArray(0); glCheckError();
		// TexCoord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); glCheckError();
		glEnableVertexAttribArray(1); glCheckError();

		glBindVertexArray(0); // Unbind VAO
		glCheckError();
	}

	glGenVertexArrays(1, &_vaoBox); glCheckError();
	glGenBuffers(1, &_vboBox); glCheckError();

	glBindVertexArray(_vaoBox); glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, _vboBox); glCheckError();
	glBufferData(GL_ARRAY_BUFFER, geometry::getCubeVerticesSize(), geometry::getCubeVertices(), GL_STATIC_DRAW); glCheckError();

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); glCheckError();
	glEnableVertexAttribArray(0); glCheckError();
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); glCheckError();
	glEnableVertexAttribArray(1); glCheckError();

	glBindVertexArray(0); // Unbind VAO
	glCheckError();

	_shader.LoadFromFile("shaders/textured_transform.vs", "shaders/textured_transform.fs");
	_textured.LoadFromFile("shaders/textured_transform.vs", "shaders/textured_no_color.fs");

	_texture_0.Load("textures/crate.png");
	_texture_1.Load("textures/awesomeface.png");
	_texture_2.Load("textures/wall.jpg");
}

void SandboxApp::Update(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCheckError();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glCheckError();

	// Draw plane
	_textured.Bind();
	_textured.BindTexture2D(0, _texture_2.GetHandle());

	glm::vec3 cameraPos{1.5f, 1.5f, 3.0f};
	glm::mat4 viewMatrix = glm::lookAt(cameraPos, { 0.0f, 0.0f, 0.0f }, glm::vec3(0.0f, 1.0f, 0.0f));

	const float aspectRatio = renderer->GetAspectRatio();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	{
		glm::mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, { 8.0f, 1.0f, 8.0f });

		glm::mat4 transform = projectionMatrix * viewMatrix * modelMatrix;
		_textured.SetUniform("transform", transform);
	}

	glBindVertexArray(_vao); glCheckError();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); glCheckError();
	glBindVertexArray(0); glCheckError();
	_textured.Unbind();

	// Draw box
	_shader.Bind();
	_shader.BindTexture2D(0, _texture_0.GetHandle());
	_shader.BindTexture2D(1, _texture_1.GetHandle());

	{
		glm::mat4 modelMatrix{ 1.0f };
		modelMatrix = glm::rotate(modelMatrix, glm::radians <float>(utils::GetTime() * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 transform = projectionMatrix * viewMatrix * modelMatrix;
		_shader.SetUniform("transform", transform);
	}

	glBindVertexArray(_vaoBox); glCheckError();
	glDrawArrays(GL_TRIANGLES, 0, 36); glCheckError();
	glBindVertexArray(0); glCheckError();
	_shader.Unbind();
}

