#include "pch.h"
#include "SpotLight.h"


SpotLight::SpotLight():PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(glm::vec3 lColor, GLfloat aIntensity, GLfloat dIntensity,
	glm::vec3 pos,
	glm::vec3 dir,
	GLfloat con, GLfloat lin, GLfloat exp,
	GLfloat edg)
	:PointLight(lColor, aIntensity, dIntensity, pos, con, lin, exp)
{

	direction = glm::normalize(glm::vec3(dir.x, dir.y, dir.z));

	edge = edg;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
	GLuint positionLocation,
	GLuint directionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
	GLuint edgeLocation)
{
	glUniform3f(colourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);

	glUniform1f(constantLocation, aConstant);
	glUniform1f(linearLocation, aLinear);
	glUniform1f(exponentLocation, aExponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{
}
