#pragma once

#include <iostream>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
#include "Vertex.h"
enum direction{FORWARD = 0,
				BACKWARD,
				LEFT,
				RIGHT, 
				UP,
				DOWN};

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

	void UpdateCameraVectors()
	{
		this->front.x = -cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front.z = sin(glm::radians(this->pitch));
		this->front.y = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->front = glm::normalize(this->front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));

	}

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
	{
		this->ViewMatrix = glm::mat4(1.f);
		this->movementSpeed = 3.f;
		this->sensitivity = 5.f;

		this->worldUp = worldUp;
		this->position = position;
		this->right = glm::vec3(0.f);
		this->up = worldUp;
		 
		this->pitch = 0.f;
		this->yaw = 0.f;
		this->roll = 0.f;

		this->UpdateCameraVectors();
	}
	~Camera()
	{
	}

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
	void move(const float& dt, const int direction)
	{
		glm::vec3 ChangePos = glm::vec3(this->front.x, 0.f, this->front.z);
		switch (direction)
		{
		case FORWARD:
			this->position += this->front * this->movementSpeed * dt;
			break;
		case BACKWARD:
			this->position -= this->front * this->movementSpeed * dt;
			break;
		case LEFT:
			this->position -= this->right * this->movementSpeed * dt;
			break;
		case RIGHT:
			this->position += this->right * this->movementSpeed * dt;
			break;
		case UP:
			this->position.z += 1.f * this->movementSpeed * dt;
			break;
		case DOWN:
			this->position.z -= 1.f * this->movementSpeed * dt;
			break;
		default:
			break;
		}
	}
	void SetPos(glm::vec3 NewPos)
	{
		this->position = NewPos;
	}
	void updateMouseInput(const float& dt, const float& offsetX, const float& offsetY)
	{
		//Update pitch, yaw and roll
		this->pitch += static_cast<GLfloat>(offsetY) * this->sensitivity *dt;
		this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity *dt;
		if (this->pitch > 80.f)
			this->pitch = 80.f;
		else if (this->pitch < -90.f)
			this->pitch = -89.9f;

		if (this->yaw > 360.f || this->yaw < -360.f)
			this->yaw = 0.f;
	}

	void updateInput(const float& dt, const int direction, MouseItems MouseOffset)
	{
		this->updateMouseInput(dt, MouseOffset.X, MouseOffset.Y);
	}
};