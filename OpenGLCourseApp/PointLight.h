#pragma once
#include "Light.h"

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight( glm::vec3 lColor, GLfloat aIntensity, GLfloat dIntensity,
				glm::vec3 pos,
				GLfloat con, GLfloat lin, GLfloat exp);
	
	void UseLight(	GLuint colourLocation, GLuint ambientIntensityLocation,	GLuint diffuseIntensityLocation,
					GLuint positionLocation,
					GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
	~PointLight();

protected:
	glm::vec3 position;

	//	Needed for calculating Attenuation factor
	GLfloat aConstant, aLinear, aExponent;
};