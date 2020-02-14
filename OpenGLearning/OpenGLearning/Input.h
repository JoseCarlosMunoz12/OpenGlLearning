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
class Input
{
private:
	void ShowGenInfo()
	{
		int Amount;
		const float* axis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &Amount);
		ImGui::Text("number of Axis %d",Amount);
		ImGui::Text("Left analog %.3f -- %.3f", axis[0], axis[1]);
		ImGui::Text("Right analog %.3f -- %.3f", axis[2], axis[3]);
		ImGui::Text("Left trigger %.3f",axis[4]);
		ImGui::Text("Right trigger %.3f", axis[5]);
		int BttnAmount;
		const unsigned char* Set = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &BttnAmount);
		ImGui::Text("Number of Buttons %d",BttnAmount);
		//Face Buttons
		if (GLFW_PRESS == Set[0])
		{
			ImGui::Text("A Pressed");
		}
		else if (GLFW_RELEASE == Set[0])
		{
			ImGui::Text("A Released");
		}
		if (GLFW_PRESS == Set[1])
		{
			ImGui::Text("B Pressed");
		}
		else if (GLFW_RELEASE == Set[1])
		{
			ImGui::Text("B Released");
		}
		if (GLFW_PRESS == Set[2])
		{
			ImGui::Text("X Pressed");
		}
		else if (GLFW_RELEASE == Set[2])
		{
			ImGui::Text("X Released");
		}
		if (GLFW_PRESS == Set[3])
		{
			ImGui::Text("Y Pressed");
		}
		else if (GLFW_RELEASE == Set[3])
		{
			ImGui::Text("Y Released");
		}
		// Bumpers
		if (GLFW_PRESS == Set[4])
		{
			ImGui::Text("Left Bumper Pressed");
		}
		else if (GLFW_RELEASE == Set[4])
		{
			ImGui::Text("Left Bumper Released");
		}
		if (GLFW_PRESS == Set[5])
		{
			ImGui::Text("Right Bumper Pressed");
		}
		else if (GLFW_RELEASE == Set[5])
		{
			ImGui::Text("Right Bumper Released");
		}
		// center Buttons
		if (GLFW_PRESS == Set[6])
		{
			ImGui::Text("View Pressed");
		}
		else if (GLFW_RELEASE == Set[6])
		{
			ImGui::Text("View Released");
		}
		if (GLFW_PRESS == Set[7])
		{
			ImGui::Text("Menu Pressed");
		}
		else if (GLFW_RELEASE == Set[7])
		{
			ImGui::Text("Menu Released");
		}
		//Analog Click
		if (GLFW_PRESS == Set[8])
		{
			ImGui::Text("Left Click Pressed");
		}
		else if (GLFW_RELEASE == Set[8])
		{
			ImGui::Text("Left Click Released");
		}
		if (GLFW_PRESS == Set[9])
		{
			ImGui::Text("Right Click Pressed");
		}
		else if (GLFW_RELEASE == Set[9])
		{
			ImGui::Text("Right Click Released");
		}
		//D Pad
		if (GLFW_PRESS == Set[10])
		{
			ImGui::Text("Up Pressed");
		}
		else if (GLFW_RELEASE == Set[10])
		{
			ImGui::Text("Up Released");
		}
		if (GLFW_PRESS == Set[11])
		{
			ImGui::Text("Right Pressed");
		}
		else if (GLFW_RELEASE == Set[11])
		{
			ImGui::Text("Right Released");
		}
		if (GLFW_PRESS == Set[12])
		{
			ImGui::Text("Down Pressed");
		}
		else if (GLFW_RELEASE == Set[12])
		{
			ImGui::Text("Down Released");
		}
		if (GLFW_PRESS == Set[13])
		{
			ImGui::Text("Left Pressed");
		}
		else if (GLFW_RELEASE == Set[13])
		{
			ImGui::Text("Left Released");
		}
	}
public:
	Input()
	{

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
};