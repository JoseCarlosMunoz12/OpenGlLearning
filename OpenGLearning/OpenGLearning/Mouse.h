#pragma once
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <mat4x4.hpp>

#include "MipMap.h"
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
	glm::vec3 MouseRay(FrameBufferItems FrameBufffer,
						glm::mat4 ProjectionMatrix,  glm::mat4 ViewMatrix)
	{
		glm::vec2 NormalizeCoord;
		NormalizeCoord.x = (2.f * this->MouseX / float(FrameBufffer.Width)) - 1.f;
		NormalizeCoord.y = 1.f - (2.f * this->MouseY / float(FrameBufffer.Height));
		glm::vec4 ScreenPos = glm::vec4(NormalizeCoord, -1.f, 1.f);
		glm::vec4 RayEye = glm::inverse(ProjectionMatrix) * ScreenPos;
		RayEye = glm::vec4(RayEye.x, RayEye.y, -1, 0);
		glm::vec4 Temp = glm::inverse(ViewMatrix) * RayEye;
		glm::vec3 WorldSpace = glm::vec3(Temp.x, Temp.y, Temp.z);
		return glm::normalize(WorldSpace);
	}
	MousePositions getMousPos()
	{
		return {this->MouseX, this->MouseY};
	}
	void SetMouseCenter(GLFWwindow* window, int WindowWidth, int WindowHeight)
	{
		this->MouseX = WindowWidth / 2.f;
		this->MouseY = WindowHeight / 2.f;
		glfwSetCursorPos(window, this->MouseX, this->MouseY);
	}

	glm::vec3 NewPosition(MipMap MapToFind,FrameBufferItems FrameBuffer,
						glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, glm::vec3 MousePosition)
	{
		glm::vec3 RayToUse = this->MouseRay(FrameBuffer, ProjectionMatrix, ViewMatrix);


	}
private:
	glm::vec3 BinarySearch(int count, float start, float finish, glm::vec3 Ray)
	{
		float half = start + ( (finish - start) / 2.f);

	}
	 
};