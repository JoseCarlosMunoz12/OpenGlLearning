#pragma once
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <mat4x4.hpp>

#include "ScreenCollision.h"
#include "MipMap.h"
#include "Vertex.h"

class Mouse
{
private:
	//Basic Mouse Details and information
	double lastMouseX;
	double lastMouseY;
	MousePositions LastMouse;
	MousePositions CurrentMouse;
	MouseItems MouseOffset;
	double MouseX;
	double MouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse = true;
	float Amount = 0;
	float AmountZ = 0;
	//2D information
	bool In2d;
	//MouseClicks and Buttons use.
	int BttnClicked;
	int OldState;
	int NewState;
	//Collision class
	UICollision UICol;
	//Variables for the binary Search
	int RECURSION_COUNT = 200;
public:
	Mouse()
	{

	}

	~Mouse()
	{

	}

//2d related functions  collision of mouse with GUI
bool UpdateMouse2dInput(GLFWwindow* window,ImGuiItems IGItems)
	{
		return this->UICol.ImGuiCollisionDetection(IGItems,this->CurrentMouse);
	}
//3d related functions
void UpdateMouseInput(GLFWwindow* window)
{
	glfwGetCursorPos(window, &this->CurrentMouse.X, &this->CurrentMouse.Y);
	if (this->firstMouse)
	{
		this->LastMouse = this->CurrentMouse;
		this->firstMouse = false;
	}
	//Calc offset
	this->MouseOffset.X = this->CurrentMouse.X - this->LastMouse.X;
	this->MouseOffset.Y = this->LastMouse.Y - this->CurrentMouse.Y;
	this->LastMouse = this->CurrentMouse;
}
//Angle information and offsets
	MouseItems GetOffset()
{
	return this->MouseOffset;
}

	glm::vec3 MouseRay(FrameBufferItems FrameBufffer,
		glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{
	glm::vec2 NormalizeCoord;
	NormalizeCoord.x = (2.f * this->CurrentMouse.X / float(FrameBufffer.Width)) - 1.f;
	NormalizeCoord.y = 1.f - (2.f * this->CurrentMouse.Y / float(FrameBufffer.Height));
	glm::vec4 ScreenPos = glm::vec4(NormalizeCoord, -1.f, 1.f);
	glm::vec4 RayEye = glm::inverse(ProjectionMatrix) * ScreenPos;
	RayEye = glm::vec4(RayEye.x, RayEye.y, -1, 0);
	glm::vec4 Temp = glm::inverse(ViewMatrix) * RayEye;
	glm::vec3 WorldSpace = glm::vec3(Temp.x, Temp.y, Temp.z);
	return glm::normalize(WorldSpace);
}

	MousePositions getMousPos()
{
	return this->CurrentMouse;
}

	void SetMouseCenter(GLFWwindow* window, int WindowWidth, int WindowHeight)
{
	this->MouseX = WindowWidth / 2.f;
	this->MouseY = WindowHeight / 2.f;
	glfwSetCursorPos(window, this->MouseX, this->MouseY);
}

	glm::vec3 NewPosition(MipMap* MapToFind, FrameBufferItems FrameBuffer,
		glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, glm::vec3 CamPosition)
{
	glm::vec3 RayToUse = this->MouseRay(FrameBuffer, ProjectionMatrix, ViewMatrix);
	glm::vec3 NewPos = this->BinarySearch(0, 0, 200, RayToUse, CamPosition, MapToFind);
	NewPos.y = MapToFind->ReturnValue(NewPos.x, NewPos.z);
	return NewPos;
}

//Mouse Button Detection Button
	bool MouseButtonClicked(GLFWwindow* window)
{
	if (this->firstMouse)
	{
	this->OldState = GLFW_RELEASE;

	}
	this->NewState = glfwGetMouseButton(window, this->ButtonFound(window));

	bool Found = ( NewState == GLFW_RELEASE && OldState == GLFW_PRESS);
	this->OldState = this->NewState;
	return Found;
}

	bool MouseButtonClicked(GLFWwindow* window, int MouseButton)
{
	if (this->firstMouse)
	{
		this->OldState = GLFW_RELEASE;

	}
	this->NewState = glfwGetMouseButton(window, MouseButton);

	bool Found = (NewState == GLFW_RELEASE && OldState == GLFW_PRESS);
	this->OldState = this->NewState;
	return Found;
}

	bool MouseButtonHold(GLFWwindow* window)
{
	return glfwGetMouseButton(window, this->ButtonFound(window)) == GLFW_PRESS;
}

	bool MouseButtonHold(GLFWwindow* window, int MouseButton)
{
	return glfwGetMouseButton(window, MouseButton) == GLFW_PRESS;
}


private:
	glm::vec3 BinarySearch(int count, float start, float finish, glm::vec3 Ray, glm::vec3 CamPosition, MipMap* Map)
	{
		float half = start + ((finish - start) / 2.f);
		if (count >= this->RECURSION_COUNT)
		{
			return this->GetPointOnRay(Ray, half, CamPosition);
		}

		if (Intersection(start, half, Ray, Map, CamPosition))
		{
			return this->BinarySearch(count + 1, start, half, Ray, CamPosition, Map);
		}
		else
		{
			return this->BinarySearch(count + 1, half, finish, Ray, CamPosition, Map);
		}
	}

	glm::vec3 GetPointOnRay(glm::vec3 Ray, float Distance, glm::vec3 CamPosition)
	{
		return CamPosition + Ray * Distance;
	}

	bool Intersection(float start, float finish, glm::vec3 Ray, MipMap* Map, glm::vec3 CamPos)
	{
		glm::vec3 StartPoint = this->GetPointOnRay(Ray, start, CamPos);
		glm::vec3 EndPoint = this->GetPointOnRay(Ray, finish, CamPos);
		if (!this->IsUnderGround(StartPoint, Map) && this->IsUnderGround(EndPoint, Map))
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

	int ButtonFound(GLFWwindow* window)
	{
		if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{
			return GLFW_MOUSE_BUTTON_1;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		{
			return GLFW_MOUSE_BUTTON_2;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
		{
			return GLFW_MOUSE_BUTTON_3;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS)
		{
			return GLFW_MOUSE_BUTTON_4;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5) == GLFW_PRESS)
		{
			return GLFW_MOUSE_BUTTON_5;;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_6) == GLFW_PRESS)
		{
			return GLFW_MOUSE_BUTTON_6;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_7) == GLFW_PRESS)
		{
			return GLFW_MOUSE_BUTTON_7;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_8) == GLFW_PRESS)
		{
			return GLFW_MOUSE_BUTTON_8;
		}
		return 8;
	}
};