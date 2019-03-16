#include "pch.h"
#include "Light.h"


Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(glm::vec3 ambientColor, GLfloat aIntensity, glm::vec3 lightDirection, GLfloat dIntensity)
{
	colour = ambientColor;
	ambientIntensity = aIntensity;

	direction = lightDirection;
	diffuseIntensity = dIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	GLfloat dIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(dIntensityLocation, diffuseIntensity);
}

Light::~Light()
{
}
