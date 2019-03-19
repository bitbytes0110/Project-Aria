#include "pch.h"
#include "PointLight.h"


PointLight::PointLight()
	:Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	aConstant = 1.0f;
	aLinear = 0.0f;
	aExponent = 0.0f;
}

PointLight::PointLight(	glm::vec3 lColor, GLfloat aIntensity, GLfloat dIntensity,
						glm::vec3 pos,
						GLfloat con, GLfloat lin, GLfloat exp)
	:Light(lColor, aIntensity, dIntensity)
{
	position = pos;
	aConstant = con;
	aLinear = lin;
	aExponent = exp;
}

void PointLight::UseLight(	GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
							GLuint positionLocation,
							GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	glUniform3f(colourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);

	glUniform1f(constantLocation, aConstant);
	glUniform1f(linearLocation, aLinear);
	glUniform1f(exponentLocation, aExponent);
}

PointLight::~PointLight()
{
}