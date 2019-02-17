
#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/mat4x4.hpp>

// Window Dimensions
const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shaderProgram, UniformXMove;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement= 0.005f;



//	Vertex Shader
static const char* vshader = "												\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec3 pos;											\n\
																			\n\
uniform float xMove;														\n\
																			\n\
void main()																	\n\
{																			\n\
	gl_Position = vec4(0.4*pos.x + xMove, 0.4*pos.y, 0.4*pos.z, 1.0);		\n\
}";


//	Fragement Shader--  We dont generally pass anything to the fragment shader we do it in the vertex shader and get it later
static const char* fshader = "				\n\
#version 330								\n\
											\n\
out vec4 color;								\n\
											\n\
void main()									\n\
{											\n\
	color = vec4(1.0, 0.0, 0.0, 1.0);		\n\
}";

using namespace std;

void CreateTriangle()
{
	GLfloat vertices[] =
	{
		-1.0f,	-1.0f,	0.0f,
		1.0f,	-1.0f,	0.0f,
		0.0f,	1.0f,	0.0f
	};

	glGenVertexArrays(1, &VAO);	// Creates memory in GPU for VAO and gives you an ID back to you in VAO
	glBindVertexArray(VAO);	// all futures operations will happen on this bound VAO

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);	// Unbinding

	glBindVertexArray(0);	// Unbinding
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		cout << "Error Compiling the "<< shaderType <<" shader : "<< eLog << endl;
		return;
	}

	glAttachShader(theProgram, theShader);


	return;
}

void CompileShaders()
{
	shaderProgram = glCreateProgram();
	if (!shaderProgram)
	{
		cout << "Error creating shader Program!\n" << endl;
		return;
	}
	AddShader(shaderProgram, vshader, GL_VERTEX_SHADER);
	AddShader(shaderProgram, fshader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram,sizeof(eLog), NULL, eLog);
		cout<<"Error Linking Program : "<< eLog <<endl;
		return;
	}

	glValidateProgram(shaderProgram);	// Checking if the shader program is valid in the current context OpenGL is working in
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		cout << "Error Validating Shader Program : " << eLog << endl;
		return;
	}

	UniformXMove = glGetUniformLocation(shaderProgram, "xMove");

	}


int main()
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

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		cout << "GLFW Window creation failed!" << endl;
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//WIDTH is the total size of the window 
	//Bufferwidth is the actual drawable area of the window

	// Set the context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW initialization failed!" << endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport Size
	glViewport(0,0, bufferWidth, bufferHeight);


	CreateTriangle();
	CompileShaders();

	//Loop until window Closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Get + Handle user input events
		glfwPollEvents();	// detects keyboard presses, mouse movements, resizing window or anyother window events

		// Changing the offset values every frame
		if (direction) 
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxoffset)
		{
			direction = !direction;
		}


		//clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // rgb vlues rangse from 0-1 NOT 1 -255
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);	// Assigning the shader

			glUniform1f(UniformXMove, triOffset);
			glBindVertexArray(VAO);	// Binding VAO
			glDrawArrays(GL_TRIANGLES,0,3);// We can have multiple objects stored in 1 VAO that starts and ends in different points. But here its just simple & compact

			glBindVertexArray(0);	// UnBinding VAO


		glUseProgram(0);	// Unassigning the shader


		glfwSwapBuffers(mainWindow);

	}

	return 0;
}

