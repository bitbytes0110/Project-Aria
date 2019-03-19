#include "pch.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(	glm::vec3 lColor, GLfloat aIntensity, GLfloat dIntensity,
									glm::vec3 lightDirection) 
	: Light(lColor, aIntensity, dIntensity)
{
	direction = lightDirection;
}

void DirectionalLight::UseLight(GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
								GLuint directionLocation)
{
	glUniform3f(colourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

DirectionalLight::~DirectionalLight()
{
}