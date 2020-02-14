#pragma once

#include <glfw3.h>
#include <glm.hpp>

#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <vector>
class Input
{
private:
	int Amount;
	const float* Axis;
	int BttnAmount;
	const unsigned char* Bttns;
	void ShowGenInfo()
	{
		ImGui::Text("number of Axis %d",Amount);
		ImGui::Text("Left analog %.3f -- %.3f", Axis[0], Axis[1]);
		ImGui::Text("Right analog %.3f -- %.3f", Axis[2], Axis[3]);
		ImGui::Text("Left trigger %.3f",Axis[4]);
		ImGui::Text("Right trigger %.3f", Axis[5]);
		ImGui::Text("Number of Buttons %d",BttnAmount);
		//Face Buttons
		if (GLFW_PRESS == Bttns[0])
		{
			ImGui::Text("A Pressed");
		}
		else if (GLFW_RELEASE == Bttns[0])
		{
			ImGui::Text("A Released");
		}
		if (GLFW_PRESS == Bttns[1])
		{
			ImGui::Text("B Pressed");
		}
		else if (GLFW_RELEASE == Bttns[1])
		{
			ImGui::Text("B Released");
		}
		if (GLFW_PRESS == Bttns[2])
		{
			ImGui::Text("X Pressed");
		}
		else if (GLFW_RELEASE == Bttns[2])
		{
			ImGui::Text("X Released");
		}
		if (GLFW_PRESS == Bttns[3])
		{
			ImGui::Text("Y Pressed");
		}
		else if (GLFW_RELEASE == Bttns[3])
		{
			ImGui::Text("Y Released");
		}
		// Bumpers
		if (GLFW_PRESS == Bttns[4])
		{
			ImGui::Text("Left Bumper Pressed");
		}
		else if (GLFW_RELEASE == Bttns[4])
		{
			ImGui::Text("Left Bumper Released");
		}
		if (GLFW_PRESS == Bttns[5])
		{
			ImGui::Text("Right Bumper Pressed");
		}
		else if (GLFW_RELEASE == Bttns[5])
		{
			ImGui::Text("Right Bumper Released");
		}
		// center Buttons
		if (GLFW_PRESS == Bttns[6])
		{
			ImGui::Text("View Pressed");
		}
		else if (GLFW_RELEASE == Bttns[6])
		{
			ImGui::Text("View Released");
		}
		if (GLFW_PRESS == Bttns[7])
		{
			ImGui::Text("Menu Pressed");
		}
		else if (GLFW_RELEASE == Bttns[7])
		{
			ImGui::Text("Menu Released");
		}
		//Analog Click
		if (GLFW_PRESS == Bttns[8])
		{
			ImGui::Text("Left Click Pressed");
		}
		else if (GLFW_RELEASE == Bttns[8])
		{
			ImGui::Text("Left Click Released");
		}
		if (GLFW_PRESS == Bttns[9])
		{
			ImGui::Text("Right Click Pressed");
		}
		else if (GLFW_RELEASE == Bttns[9])
		{
			ImGui::Text("Right Click Released");
		}
		//D Pad
		if (GLFW_PRESS == Bttns[10])
		{
			ImGui::Text("Up Pressed");
		}
		else if (GLFW_RELEASE == Bttns[10])
		{
			ImGui::Text("Up Released");
		}
		if (GLFW_PRESS == Bttns[11])
		{
			ImGui::Text("Right Pressed");
		}
		else if (GLFW_RELEASE == Bttns[11])
		{
			ImGui::Text("Right Released");
		}
		if (GLFW_PRESS == Bttns[12])
		{
			ImGui::Text("Down Pressed");
		}
		else if (GLFW_RELEASE == Bttns[12])
		{
			ImGui::Text("Down Released");
		}
		if (GLFW_PRESS == Bttns[13])
		{
			ImGui::Text("Left Pressed");
		}
		else if (GLFW_RELEASE == Bttns[13])
		{
			ImGui::Text("Left Released");
		}
	}
public:
	Input()
	{

	}
	void UpdateVals()
	{
		Bttns = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &BttnAmount);
		Axis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &Amount);
	}
	void RenderBox()
	{
		ImGui::Begin("Controller information");
		if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE)
		{
			ImGui::Text("Found");
			this->ShowGenInfo();
		}
		else
		{
			ImGui::Text("Not Found");
		}
		ImGui::End();
	}
	bool ControllerExist()
	{
		return glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE;
	}
	std::vector<float> GetLeftAnalogVals()
	{
		return {Axis[0],Axis[1]};
	}
	std::vector<float> GetRightAnalogVals()
	{
		return { Axis[2],Axis[3] };
	}
	float GetLeftTrigVal()
	{	
		return Axis[4];
	}
	float GetRightTrigVal()
	{
		return Axis[5];
	}
};