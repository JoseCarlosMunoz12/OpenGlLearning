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
	//Variables for the binary Search
	int RECURSION_COUNT = 200;
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

	glm::vec3 NewPosition(MipMap* MapToFind,FrameBufferItems FrameBuffer,
						glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, glm::vec3 CamPosition)
	{
		glm::vec3 RayToUse = this->MouseRay(FrameBuffer, ProjectionMatrix, ViewMatrix);
		glm::vec3 NewPos = this->BinarySearch(0, 0, 200, RayToUse, CamPosition,MapToFind);
		NewPos.y = MapToFind->ReturnValue(NewPos.x, NewPos.z);
		return NewPos;
	}
private:
	glm::vec3 BinarySearch(int count, float start, float finish, glm::vec3 Ray, glm::vec3 CamPosition,MipMap* Map)
	{
		float half = start + ( (finish - start) / 2.f);
		if (count >= this->RECURSION_COUNT)
		{
			return this->GetPointOnRay(Ray, half, CamPosition);
		}

		if (Intersection(start, half, Ray,Map,CamPosition))
		{
			return this->BinarySearch(count + 1, start, half, Ray, CamPosition, Map);
		}
		else
		{
			return this->BinarySearch(count + 1, half, finish, Ray, CamPosition, Map);
		}
	}
	glm::vec3 GetPointOnRay(glm::vec3 Ray, float Distance,glm::vec3 CamPosition)
	{
		return CamPosition + Ray * Distance;
	}
	bool Intersection(float start, float finish, glm::vec3 Ray,MipMap* Map,glm::vec3 CamPos)
	{
		glm::vec3 StartPoint = this->GetPointOnRay(Ray, start, CamPos);
		glm::vec3 EndPoint = this->GetPointOnRay(Ray, finish, CamPos);
		if (!this->IsUnderGround(StartPoint, Map) && this->IsUnderGround(EndPoint,Map))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsUnderGround(glm::vec3 TestPoint, MipMap* Map)
	{
		float height = Map->ReturnValue(TestPoint.x, TestPoint.z);
		return TestPoint.y < height;
	}
};