#include "pch.h"
#include "Window.h"


Window::Window()
{
	width = WIDTH;
	height = HEIGHT;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}



int Window::Initialise()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		cout << "GLFW initialization failed!" << endl;
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwards compatilbility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Aria", NULL, NULL);
	if (!mainWindow)
	{
		cout << "GLFW Window creation failed!" << endl;
		glfwTerminate();
		return 1;
	}

	
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);


	// Set the context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Callback for resizing the buffers when resizing the window
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);


	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW initialization failed!" << endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}



	glEnable(GL_DEPTH_TEST);

	// Setup viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	return 0;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
