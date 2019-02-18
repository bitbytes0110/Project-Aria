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


};

