#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

using namespace std;

const float toRadians = 3.14159265f	/	180.0f;

Window mainWindow;

vector<Mesh*> MeshList;
vector<Shader*> ShaderList;

//	Vertex Shader
static const char* vShaderFileLocation = "Shaders/shader.VS";

//	Fragement Shader--  We dont generally pass anything to the fragment shader we do it in the vertex shader and get it later
static const char* fShaderFileLocation = "Shaders/shader.FS";

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

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShaderFileLocation, fShaderFileLocation);
	ShaderList.push_back(shader1);
}

void processInput(GLFWwindow* mainWindow)
{
	// MOved!
}

int main()
{
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	//Putting this outside the loop cause once you set it, you don't have to recalculate it often
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/ mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Loop until window Closed
	while (!mainWindow.getShouldClose())
	{
		//Get + Handle user input events
		processInput(mainWindow.getMainWindow());

		glfwPollEvents();	// detects keyboard presses, mouse movements, resizing window or anyother window events

		//clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // State Setting function rgb vlues rangse from 0-1 NOT 1 -255
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// State using function

		ShaderList[0]->UseShader();
		uniformModel = ShaderList[0]->GetModelLocation();
		uniformProjection = ShaderList[0]->GetProjectionLocation();

		glm::mat4 model = glm::mat4(1.0f);			//Identity Matrix
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
		MeshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f,  -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));	
		MeshList[1]->RenderMesh();
		
		glUseProgram(0);	// Unassigning the shader

		mainWindow.swapBuffers();
	}
	return 0;
}

