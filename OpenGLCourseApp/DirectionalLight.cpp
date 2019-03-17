#include "pch.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(	glm::vec3 lColor,
									GLfloat aIntensity, GLfloat dIntensity,
									glm::vec3 lightDirection) 
	: Light(lColor, aIntensity, dIntensity)
{
	direction = lightDirection;
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
	GLuint dIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(dIntensityLocation, diffuseIntensity);
}



DirectionalLight::~DirectionalLight()
{
}
