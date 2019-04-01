#pragma once

#include <iostream>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>

class Camera
{
private:
	glm::mat4 ViewMatrix;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	void UpdateCameraVectors();

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
	{

	}
	~Camera();

	//Acessors
	const glm::mat4 GetViewMatrix()
	{
		this->UpdateCameraVectors();
		this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
		return this->ViewMatrix;
	}

	const glm::vec3 getPosition() const
	{
		return this->position;
	}
	//Functions
	void updateKeyboardInpuit(const float& dt, const int direction)
	{
		//update position vector
	}

	void updateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
	{
		//Update pitch, yaw and roll
	}

	void updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
	{
		this->updateKeyboardInpuit(dt, direction);
		this->updateMouseInput(dt, offsetX, offsetY);
	}
};