#include "pch.h"
#include "Window.h"

Window::Window()
	:Window(WIDTH, HEIGHT)
{
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	mouseFirstMoved = true;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	if (!glfwInit())
	{
		cout << "GLFW initialization failed!" << endl;
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// Core profile = No Backwards compatilbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// Allow forward compatibility

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

	// Handle Key + mouse input
	createCallbacks();

	//	Don't display mouse pointer
	//	We do this so that the mouse pointer doesn't go outside the screen
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	

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

	//	We are using this function to save the owner of the main window to GLFW and access it in a static function outside our class!
	//	this is the owner (user pointer) here. We are assigning a owner to mainWindow
	glfwSetWindowUserPointer(mainWindow, this);	

	return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);	
	glfwSetCursorPosCallback(mainWindow, handleMouse);
	glfwSetFramebufferSizeCallback(mainWindow, handleWindowResize);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	//	Now we got the owner who is using the mainWindow, so that we can access keys in this function
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;	//	here
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));// now we got the owner who is using the mainWIndow: smart!

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastX = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;	// To avoid inverted controls

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

void Window::handleWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}