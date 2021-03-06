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

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

#include "Camera.h"
#include "Texture.h"
#include "Material.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Model.h"

using namespace std;

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;

vector<Mesh*> MeshList;
vector<Shader*> ShaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


Material shinyMaterial;
Material dullMaterial;

Model xWing;
Model skull;
Model blackHawk;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

//	Vertex Shader
static const char* vShaderFileLocation = "Shaders/shader.VS";

//	Fragment Shader--  We don't generally pass anything to the fragment shader we do it in the vertex shader and get it later
static const char* fShaderFileLocation = "Shaders/shader.FS";


void calcAverageNormals(unsigned int* indices, unsigned int indiceCount,
						GLfloat* vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	//	Each iteration does one side!
	for (size_t i = 0; i < indiceCount; i += 3)	
	{
		// Access start position of vertex data of each side
		unsigned int in0 = indices[i + 0] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		//	Now we take any 2 edges and do a cross product to get a normal
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		//	Change the normal data(0, 0, 0) we have with the correct normal we calculated above
		//	Apply the normal to all the 3 points of the side
		vertices[in0 + normalOffset + 0] += normal.x;
		vertices[in0 + normalOffset + 1] += normal.y;
		vertices[in0 + normalOffset + 2] += normal.z;

		vertices[in1 + normalOffset + 0] += normal.x;
		vertices[in1 + normalOffset + 1] += normal.y;
		vertices[in1 + normalOffset + 2] += normal.z;

		vertices[in2 + normalOffset + 0] += normal.x;
		vertices[in2 + normalOffset + 1] += normal.y;
		vertices[in2 + normalOffset + 2] += normal.z;
	}

	//	Normalize the normal data
	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset + 0], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset + 0] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] =
	{
		0,	3,	1,	// Side 1 Front Left
		1,	3,	2,	// Side 2 Front Right
		2,	3,	0,	// Side 3 Back
		0,	1,	2	// Side 4 Bottom
	};

	GLfloat vertices[] =
	{
	// 	x		y		z			u		v			nx		ny		nz
		-1.0f,	-1.0f,	-0.6f,		0.0f,	0.0f,		0.0f,	0.0f,	0.0f,
		0.0f,	-1.0f,	1.0f,		0.5f,	0.0f,		0.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,	-0.6f,		1.0f,	0.0f,		0.0f,	0.0f,	0.0f,
		0.0f,	1.0f,	0.0f,		0.5f,	1.0f,		0.0f,	0.0f,	0.0f
	};

	unsigned int floorIndices[] =
	{
		0,	2,	1,
		1,	2,	3,
	};

	GLfloat floorVertices[] =
	{														// -1.0 ?? How
		-10.0f, 0.0f, -10.0f,		0.0f,	0.0f,		0.0f,	-1.0f,	0.0f,	//	Back Left
		10.0f, 0.0f, -10.0f,		10.0f,	0.0f,		0.0f,	-1.0f,	0.0f,	//	Back Right
		-10.0f, 0.0f, 10.0f,		0.0f,	10.0f,		0.0f,	-1.0f,	0.0f,	//	Front Left
		10.0f, 0.0f, 10.0f,			10.0f,	10.0f,		0.0f,	-1.0f,	0.0f,	//	Front Right
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	MeshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	MeshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	MeshList.push_back(obj3);
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

	camera = Camera((glm::vec3(0.0f, 0.0f, 0.0f)), -90.0f, 0.0f, CAMERA_SPEED, CAMERA_SENSITIVITY);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 8);

	xWing = Model();
	xWing.LoadModel("Models/x-wing.obj");


	skull = Model();
	skull.LoadModel("Models/12140_Skull_v3_L2.obj");

	


	blackHawk = Model();
	blackHawk.LoadModel("Models/uh60.obj");

	mainLight = DirectionalLight(	glm::vec3(1.0f,	1.0f, 1.0f),	//	Color
										0.2f,	0.6f,				//	Ambient and Diffuse intensity
									glm::vec3(0.0f, 0.0f, -1.0f));	//	Direction

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(	glm::vec3(0.0f, 0.0f, 1.0f),	//	Color
										0.0f,	0.1f,				//	Ambient and Diffuse intensity
									glm::vec3(4.0f, 0.0f, 0.0f),	//	Position
										0.3f,	0.2f,	0.1f);		//	Attenuation
	pointLightCount++;
	
	pointLights[1] = PointLight(glm::vec3(0.0f, 1.0f, 0.0f),	
										0.0f,	0.1f,			
									glm::vec3(-4.0f, 2.0f, 0.0f),
										0.3f,	0.1f,	0.1f);	
	pointLightCount++;

	unsigned int spotLightCount = 0;

	//Spot light attached to camera
	spotLights[0] = SpotLight(glm::vec3(1.0f, 1.0f, 1.0f),	//	Color
		0.0f, 1.0f,				//	Ambient and Diffuse intensity
		glm::vec3(0.0f, 0.0f, 0.0f),	//	Position
		glm::vec3(0.0f, -1.0f, 0.0f),	//	Direction
		1.0f, 0.0f, 0.00f,
		20.0f);		//	Attenuation
	spotLightCount++;

	spotLights[1] = SpotLight(glm::vec3(1.0f, 1.0f, 1.0f),
		0.0f, 1.0f,				
		glm::vec3(0.0f, 1.5f, 0.0f),
		glm::vec3(-30.0f, -10.0f, 0.0f),
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	
	GLuint uniformProjectionID = 0;
	GLuint uniformModelID = 0;
	GLuint uniformViewID = 0;
	GLuint uniformEyePositionID = 0;

	GLuint uniformSpecularIntensityID = 0;
	GLuint uniformSpecularShininessID = 0;

	//Putting this outside the loop cause once you set it, you don't have to recalculate it often
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/ mainWindow.getBufferHeight(), 0.1f, 100.0f);
	   
	while (!mainWindow.getShouldClose())
	{
		//	To solve movement being faster for faster CPUs we use Delta time
		//	Delta Time : Time taken for completing one iteration
		//	We can also use a fixed frame rate to fix the solution.
		//	Here lets use Delta time

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();	// detects keyboard presses, mouse movements, resizing window or any other window events

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // State Setting function rgb values ranges from 0-1 NOT 1 -255
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// State using function

		ShaderList[0]->UseShader();
		uniformModelID = ShaderList[0]->GetModelLocation();
		uniformProjectionID = ShaderList[0]->GetProjectionLocation();
		uniformViewID = ShaderList[0]->GetViewLocation();
		uniformEyePositionID = ShaderList[0]->GetEyePositionLocation();

		uniformSpecularIntensityID = ShaderList[0]->GetSpecularIntensityLocation();
		uniformSpecularShininessID = ShaderList[0]->GetSpecularShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].setFlash(lowerLight, camera.getCameraDirection());
		
		ShaderList[0]->SetDirectionalLight(&mainLight);
		ShaderList[0]->SetPointLights(pointLights, pointLightCount);
		ShaderList[0]->SetSpotLights(spotLights, spotLightCount);

		glUniformMatrix4fv(uniformProjectionID, 1, GL_FALSE, glm::value_ptr(projection));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
		glUniformMatrix4fv(uniformViewID, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
		glUniform3f(uniformEyePositionID, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model = glm::mat4(1.0f);			//Identity Matrix
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModelID, 1, GL_FALSE, glm::value_ptr(model));	// the raw format of the model wont work, we need to give it a pointer that will work with the shader
		
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensityID, uniformSpecularShininessID);
		MeshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f,  -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModelID, 1, GL_FALSE, glm::value_ptr(model));
		
		dullMaterial.UseMaterial(uniformSpecularIntensityID, uniformSpecularShininessID);
		dirtTexture.UseTexture();
		MeshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModelID, 1, GL_FALSE, glm::value_ptr(model));

		shinyMaterial.UseMaterial(uniformSpecularIntensityID, uniformSpecularShininessID);
		plainTexture.UseTexture();
		MeshList[2]->RenderMesh();

		//3D Models 

			   
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
		glUniformMatrix4fv(uniformModelID, 1, GL_FALSE, glm::value_ptr(model));

		shinyMaterial.UseMaterial(uniformSpecularIntensityID, uniformSpecularShininessID);
		xWing.RenderModel();




		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModelID, 1, GL_FALSE, glm::value_ptr(model));

		shinyMaterial.UseMaterial(uniformSpecularIntensityID, uniformSpecularShininessID);
		skull.RenderModel();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModelID, 1, GL_FALSE, glm::value_ptr(model));

		shinyMaterial.UseMaterial(uniformSpecularIntensityID, uniformSpecularShininessID);
		blackHawk.RenderModel();





		
		glUseProgram(0);	// Unassigning the shader

		mainWindow.swapBuffers();
	}
	return 0;
}

