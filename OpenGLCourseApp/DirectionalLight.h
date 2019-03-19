#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight();

	DirectionalLight(glm::vec3 lColor, GLfloat aIntensity, GLfloat dIntensity,
					glm::vec3 lDirection);

	void UseLight(	GLuint colourLocation, GLuint aIntensityLocation, GLuint diffuseIntensityLocation,
					GLuint directionLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};