#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();
	SpotLight(glm::vec3 lColor, GLfloat aIntensity, GLfloat dIntensity,
		glm::vec3 pos,
		glm::vec3 dir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edge);

	void UseLight(GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
		GLuint positionLocation,
		GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation	);

	void setFlash(glm::vec3 pos, glm::vec3 dir);
	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};

