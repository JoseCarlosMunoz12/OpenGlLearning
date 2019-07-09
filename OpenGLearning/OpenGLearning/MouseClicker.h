#pragma once
#include "libs.h"

class Mouse
{
private:
	double lastMouseX;
	double lastMouseY;
	double MouseX;
	double MouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;
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
		std::cout << this->MouseX << "-" << this->MouseY <<"\n";
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
};