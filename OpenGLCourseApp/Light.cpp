#include "pch.h"
#include "Light.h"


Light::Light()
	:Light(glm::vec3 (1.0f, 1.0f, 1.0f), 1.0f)
{
}

Light::Light(glm::vec3 rgbColour, GLfloat aIntensity)
{
	colour = rgbColour;
	ambientIntensity = aIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}

Light::~Light()
{
}
