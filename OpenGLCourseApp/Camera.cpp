#include "pch.h"
#include "Camera.h"

Camera::Camera()
		:Camera((glm::vec3(0.0f, 0.0f, 0.0f)), -90.0f, 0.0f, CAMERA_SENSITIVITY, CAMERA_SPEED)
{
}

Camera::Camera(glm::vec3 startPosition, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed )
{
	//	Do not need a front and right vector as it can be calculated by the pitch and the yaw

	position = startPosition;
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);	//	Y-axis
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);	//	Facing -ve Z-axis

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_LEFT_SHIFT])
	{
		velocity *= 2.0f;
	}
	if (keys[GLFW_KEY_LEFT_CONTROL])
	{
		velocity *= 0.2f;
	}
	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	if (keys[GLFW_KEY_Q])
	{
		position -= up * velocity;
	}
	if (keys[GLFW_KEY_E])
	{
		position += up * velocity;
	}
	
	//	We are not changing angle, pitch and yaw won't change, so we don't need to call update
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f )
	{
		pitch = 89.0f;	//	To restrict pitching more than 90
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;	//	To restrict pitching more than -90
	}
	update();	
}

void Camera::update()
{
	//-----------------------------------------------------------------
	//				PITCH UPDATES		    *	 	YAW UPDATES		
	//-----------------------------------------------------------------
	front.x = cos(glm::radians(pitch))		*	cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));										//We are always yawing around Y axis [0 1 0]!
	front.z = cos(glm::radians(pitch))		*	sin(glm::radians(yaw));

	front = glm::normalize(front);	//	We are just removing the magnitude and keep only the direction
										
	// we want the right to be always level to the ground as we are not considering roll here
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::calculateViewMatrix()
{
	// We do all the above things only to figure out the view matrix
	glm::vec3 lookHere = position + front;	//	Can be made to look at any object
	return glm::lookAt(position, lookHere, up);
}


glm::vec3 Camera::getCameraPosition()
{
	return position;
}

Camera::~Camera()
{
}