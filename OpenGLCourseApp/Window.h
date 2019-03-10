#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#define WIDTH 1200	// <----set your Window dimensions here
#define HEIGHT 900	// <----set your Window dimensions here

#include <iostream>
using namespace std;

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLFWwindow* getMainWindow()
	{
		return mainWindow;
	}
	GLfloat getBufferWidth()
	{
		return bufferWidth;
	}
	GLfloat getBufferHeight()
	{
		return bufferHeight;
	}

	bool getShouldClose()
	{
		return glfwWindowShouldClose(mainWindow);
	}

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers()
	{
		glfwSwapBuffers(mainWindow);
	}
	~Window();

private:
	GLFWwindow* mainWindow;
	
	void static framebuffer_size_callback(GLFWwindow* window, int width, int height) // callback function
	{
		glViewport(0, 0, width, height);
	}
	GLfloat width, height;				//WIDTH is the total size of the window 
	GLint bufferWidth, bufferHeight;	//Bufferwidth is the actual drawable area of the window

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	
	void createCallbacks();

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);	// Using static as the callback go tho the member function of a class as it will be specific to an object
	// Inorder to be recognised as a callback function, it needs to have all the 5 arguments

	static void handleMouse(GLFWwindow* window, double xPos, double yPos);		// GLFW give the current coordinates
	
};