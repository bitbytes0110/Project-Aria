#pragma once

#include <GL\glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#define CAMERA_SENSITIVITY 0.08f
#define CAMERA_SPEED 5.0f


class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	
	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();

	glm::mat4 calculateViewMatrix();
	
	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 worldUp;

	GLfloat pitch;
	GLfloat yaw;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};