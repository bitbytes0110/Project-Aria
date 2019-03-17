#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight();

	DirectionalLight(glm::vec3 lColor,
					GLfloat aIntensity, GLfloat dIntensity,
					glm::vec3 lDirection);

	void UseLight(GLuint aIntensityLocation, GLuint aColourLocation,
		GLuint dIntensityLocation, GLuint directionLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};

