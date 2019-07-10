#pragma once
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <mat4x4.hpp>

#include "Vertex.h"

class Mouse
{
private:
	double lastMouseX;
	double lastMouseY;
	double MouseX;
	double MouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse = true;
	float Amount = 0;
	float AmountZ = 0;
public:
	Mouse()
	{

	}
	~Mouse()
	{

	}
	void UpdateMouseInput(GLFWwindow* window)
	{
		glfwGetCursorPos(window, &this->MouseX, &this->MouseY);
		if (this->firstMouse)
		{
			this->lastMouseX = this->MouseX;
			this->lastMouseY = this->MouseY;
			this->firstMouse = false;
		}
		//Calc offset
		this->mouseOffsetX = this->MouseX - this->lastMouseX;
		this->mouseOffsetY = this->lastMouseY - this->MouseY;
		this->lastMouseX = this->MouseX;
		this->lastMouseY = this->MouseY;
	}
	MouseItems GetOffset()
	{
		return {this->mouseOffsetX,this->mouseOffsetY};
	}
	glm::vec3 MouseRay()
	{

	}
};