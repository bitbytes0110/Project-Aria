
#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

using namespace std;

// Window Dimensions
const GLint WIDTH = 1200, HEIGHT = 900;
const float toRadians = 3.14159265f	/	180.0f;

vector<Mesh*> MeshList;
vector<Shader*> ShaderList;

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
static const char* vShader = "								\n\
#version 330												\n\
															\n\
layout (location = 0) in vec3 pos;							\n\
															\n\
uniform mat4 model;											\n\
uniform mat4 projection;									\n\
															\n\
out vec4 vCol;												\n\
															\n\
void main()													\n\
{															\n\
	gl_Position = projection * model * vec4(pos, 1.0);		\n\
	vCol= vec4(clamp(pos, 0.0f,1.0f),1.0f) ;				\n\
}";


//	Fragement Shader--  We dont generally pass anything to the fragment shader we do it in the vertex shader and get it later
static const char* fShader = "								\n\
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


void CreateObjects()
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

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	MeshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	MeshList.push_back(obj2);
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


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromString(vShader, fShader);
	ShaderList.push_back(shader1);

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

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	//Putting this outside the loop cause once you set it, you don't have to recalculate it often
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);


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

		ShaderList[0]->UseShader();
		uniformModel = ShaderList[0]->GetModelLocation();
		uniformProjection = ShaderList[0]->GetProjectionLocation();

		glm::mat4 model = glm::mat4(1.0f);			//Identity Matrix
		

		//Order is VERY Important(note it all happens in reverse order!)

		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		//The triangle looks distroted because we are NOT using projection matrix.
		//We are currently using the coordinates relative to the size of the screen.
		//Projection matrix converts the world coordinates into screen coordinates. Coming soon.

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
			
		MeshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-triOffset, 1.0f,  -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		MeshList[1]->RenderMesh();
		
		glUseProgram(0);	// Unassigning the shader


		glfwSwapBuffers(mainWindow);

	}

	return 0;
}

