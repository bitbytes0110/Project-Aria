
#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

// Window Dimensions
const GLint WIDTH = 1200, HEIGHT = 900;
GLuint VAO, VBO, IBO, shaderProgram, uniformModel;
const float toRadians = 3.14159265f	/	180.0f;

//Values for Translation
bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement= 0.005f;

//Values for Rotation
float curAngle = 0.0f;

//Values for Scaling
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//	Vertex Shader
static const char* vshader = "								\n\
#version 330												\n\
															\n\
layout (location = 0) in vec3 pos;							\n\
															\n\
uniform mat4 model;											\n\
															\n\
out vec4 vCol;												\n\
															\n\
void main()													\n\
{															\n\
	gl_Position = model * vec4(pos, 1.0);					\n\
	vCol= vec4(clamp(pos, 0.0f,1.0f),1.0f) ;				\n\
															\n\
															\n\
}";


//	Fragement Shader--  We dont generally pass anything to the fragment shader we do it in the vertex shader and get it later
static const char* fshader = "								\n\
#version 330												\n\
															\n\
in vec4 vCol;												\n\
															\n\
out vec4 color;												\n\
															\n\
void main()													\n\
{															\n\
	color = vCol;											\n\
}";

using namespace std;

void CreateTriangle()
{
	unsigned int indices[] =
	{
		0,	3,	1,
		1,	3,	2,
		2,	3,	0,
		0,	1,	2
	};

	GLfloat vertices[] =
	{
		-1.0f,	-1.0f,	0.0f,
		0.0f,	-1.0f,	1.0f, 
		1.0f,	-1.0f,	0.0f,
		0.0f,	1.0f,	0.0f
	};

	glGenVertexArrays(1, &VAO);	// Creates memory in GPU for VAO and gives you an ID back to you in VAO
	glBindVertexArray(VAO);	// all futures operations will happen on this bound VAO

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);		//Storing Indices (IBO or EBO)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);		// Just like VBO

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);	// Unbinding VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// Unbinding IBO

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

	uniformModel = glGetUniformLocation(shaderProgram, "model");

}

void updateTranslation()
{
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
}

void updateRotation()
{
	curAngle += 0.1f;
	if (curAngle >= 360)
	{
		curAngle -= 360;	// no need to reset as 361 is still considered as 1 but there is a risk of overflow
	}
}

void updateScaling()
{
	if (sizeDirection)
	{
		curSize += 0.001f;
	}
	else
	{
		curSize -= 0.001f;
	}

	if ((curSize >= maxSize) || (curSize <= minSize))
	{
		sizeDirection = !sizeDirection;
	}
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

	glEnable(GL_DEPTH_TEST);

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

		updateTranslation();
		updateRotation();
		updateScaling();

		//clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // rgb vlues rangse from 0-1 NOT 1 -255
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);	// Assigning the shader

		glm::mat4 model = glm::mat4(1.0f);;			//Identity Matrix

		//Order is VERY Important(note it all happens in reverse order!)

		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		//The triangle looks distroted because we are NOT using projection matrix.
		//We are currently using the coordinates relative to the size of the screen.
		//Projection matrix converts the world coordinates into screen coordinates. Coming soon.

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
			
			glBindVertexArray(VAO);	// Binding VAO
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO);	// Binding IBO
					//glDrawArrays(GL_TRIANGLES,0,3);// We can have multiple objects stored in 1 VAO that starts and ends in different points. But here its just simple & compact
					glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);	//  Draw using Elements/Indices
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// UnBinding IBO
			glBindVertexArray(0);	// UnBinding VAO


		glUseProgram(0);	// Unassigning the shader


		glfwSwapBuffers(mainWindow);

	}

	return 0;
}

