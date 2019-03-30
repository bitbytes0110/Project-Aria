#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <GL\glew.h>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

using namespace std;

class Shader
{
public:
	Shader();

	void	CreateFromString(const char* vertexCode, const char* fragmentCode);
	void	CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint	GetProjectionLocation();
	GLuint	GetModelLocation();
	GLuint	GetViewLocation();
	/*		TO REMOVE ??
	GLuint	GetAmbientIntensityLocation();
	GLuint	GetAmbientColourLocation();
	GLuint	GetDiffuseIntensityLocation();
	GLuint	GetDirectionLocation();
	*/
	GLuint	GetSpecularIntensityLocation();
	GLuint	GetSpecularShininessLocation();
	GLuint	GetEyePositionLocation();

	void	SetDirectionalLight(DirectionalLight* dLight);
	void	SetPointLights(PointLight* pLight, unsigned int lightCount);
	void	SetSpotLights(SpotLight* sLight, unsigned int lightCount);

	void	UseShader();
	void	ClearShader();
	~Shader();

private:
	int pointLightCount;
	int spotLightCount;
	GLuint shaderID, uniformProjectionID, uniformModelID, uniformViewID, uniformEyePosition,
		uniformSpecularIntensityID, uniformSpecularShininessID;

	struct uniformDirectionalLightStruct
	{
		GLuint	colourID;
		GLuint	aIntensityID;
		GLuint	dIntensityID;
		GLuint	directionID;
	};
	
	struct uniformPointLightStruct
	{
		GLuint	colourID;
		GLuint	aIntensityID;
		GLuint	dIntensityID;
		GLuint	positionID;

		GLuint	aConstantID;
		GLuint	aLinearID;
		GLuint	aExponentID;
	};

	struct uniformSpotLightsStruct
	{
		GLuint	colourID;
		GLuint	aIntensityID;
		GLuint	dIntensityID;
		GLuint	positionID;

		GLuint	aConstantID;
		GLuint	aLinearID;
		GLuint	aExponentID;

		GLuint	directionID;
		GLuint	edgeID;
	};

	uniformDirectionalLightStruct uniformDirectionalLight;

	uniformPointLightStruct uniformPointLights[MAX_POINT_LIGHTS];
	uniformSpotLightsStruct uniformSpotLights[MAX_SPOT_LIGHTS];
	GLuint uniformPointLightCount;
	GLuint uniformSpotLightCount;

	void	CompileShader(const char* vertexCode, const char* fragmentCode);
	void	AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};