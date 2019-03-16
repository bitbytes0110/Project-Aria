#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>


class Light
{
public:
	Light();
	Light(glm::vec3 ambientColor, GLfloat aIntensity, glm::vec3 lightDirection, GLfloat dIntensity);

	void UseLight(GLfloat aIntensityLocation, GLfloat aColourLocation, 
		GLfloat dIntensityLocation, GLfloat directionLocation );

	~Light();

private:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;


};

