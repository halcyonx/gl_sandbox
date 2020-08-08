#include <cstdlib>
#include <Log.h>
#include <Timer.h>
#ifdef PLATFORM_WINDOWS
#include "ApplicationWindows.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void RunApplication(Application& application, AppDelegate* delegate)
{
	application.Start(delegate);
}

void Application::Start(AppDelegate* delegate)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	utils::initTime();

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(2130, 1080, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR("Failed to initialize OpenGL context");
		exit(EXIT_FAILURE);
	}

	InitializeRenderer(delegate);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		MainLoopContent();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	Shutdown();
}

void Application::Shutdown()
{
	glfwTerminate();
}

#endif
