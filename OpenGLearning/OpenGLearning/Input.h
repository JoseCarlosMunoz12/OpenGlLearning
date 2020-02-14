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
		}
		else
		{
			ImGui::Text("Not Found");
		}
		ImGui::End();
	}
};