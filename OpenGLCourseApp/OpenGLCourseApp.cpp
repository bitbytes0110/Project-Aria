#include "pch.h"

#define STB_IMAGE_IMPLEMENTATION

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

#include "Camera.h"
#include "Texture.h"
#include "Light.h"


Texture brickTexture;
Texture dirtTexture;

Light mainLight;


using namespace std;

const float toRadians = 3.14159265f	/	180.0f;

Window mainWindow;

vector<Mesh*> MeshList;
vector<Shader*> ShaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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
	//	x		y		z		u		v
		-1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	-1.0f,	1.0f,	0.5f,	0.0f,
		1.0f,	-1.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.5f,	1.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	MeshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	MeshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShaderFileLocation, fShaderFileLocation);
	ShaderList.push_back(shader1);
}

int main()
{
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera((glm::vec3(1.0f, 1.0f, 1.0f)), -90.0f, 0.0f, CAMERA_SPEED, CAMERA_SENSITIVITY);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	mainLight = Light(glm::vec3(1.0f,1.0f,0.0f),0.5f);

	GLuint uniformProjectionID = 0, uniformModelID = 0, uniformViewID = 0, uniformAmbientIntensityID = 0, uniformAmbientColourID = 0;
	//Putting this outside the loop cause once you set it, you don't have to recalculate it often
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/ mainWindow.getBufferHeight(), 0.1f, 100.0f);

	while (!mainWindow.getShouldClose())
	{
		//	To solve movement being faster for faster cpus we use Delta time
		//	Delta Time : Time taken for completing one iteration
		//	We can also use a fixed frame rate to fix the solution.
		//	Here lets use Delta time

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();	// detects keyboard presses, mouse movements, resizing window or anyother window events

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // State Setting function rgb vlues rangse from 0-1 NOT 1 -255
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// State using function

		ShaderList[0]->UseShader();
		uniformModelID = ShaderList[0]->GetModelLocation();
		uniformProjectionID = ShaderList[0]->GetProjectionLocation();
		uniformViewID = ShaderList[0]->GetViewLocation();
		uniformAmbientIntensityID = ShaderList[0]->GetAmbientIntensityLocation();
		uniformAmbientColourID = ShaderList[0]->GetAmbientColourLocation();

		mainLight.UseLight(uniformAmbientIntensityID, uniformAmbientColourID);

		glm::mat4 model = glm::mat4(1.0f);			//Identity Matrix
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModelID, 1, GL_FALSE, glm::value_ptr(model));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
		glUniformMatrix4fv(uniformProjectionID, 1, GL_FALSE, glm::value_ptr(projection));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
		glUniformMatrix4fv(uniformViewID, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader

		brickTexture.UseTexture();

		MeshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f,  -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModelID, 1, GL_FALSE, glm::value_ptr(model));
		
		dirtTexture.UseTexture();
		MeshList[1]->RenderMesh();
		
		glUseProgram(0);	// Unassigning the shader

		mainWindow.swapBuffers();
	}
	return 0;
}

