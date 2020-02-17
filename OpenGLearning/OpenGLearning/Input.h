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
	struct ButtnsClick {
		float Time;
		float Prev;
		bool D_Pressed;
	};
private:
	int JoyStickID;
	int Amount;
	const float* Axis;
	int BttnAmount;
	const unsigned char* Bttns;
	float IsDouble;
	std::vector<float> R_cntr_Ac = {10.f,10.f};
	std::vector <std::string> Buttons = {"A", "B", "X", "Y",
										 "L Bumper", "R Bumper",
										 "View", "Menu",
										 "L Click", "R Click",
										 "Up", "Right", "Down", "Left"};
	std::vector<ButtnsClick> BttnHold;
	bool Connected;	
	void ShowGenInfo()
	{
		ImGui::Text("Analog Values");
		ImGui::Text("Number of Axis %d",Amount);
		ImGui::Text("Left analog %.3f -- %.3f", Axis[0], Axis[1]);
		ImGui::Text("Right analog %.3f -- %.3f", Axis[2], Axis[3]);
		ImGui::Text("Left trigger %.3f",Axis[4]);
		ImGui::Text("Right trigger %.3f", Axis[5]);
		ImGui::Text("Number of Buttons %d",BttnAmount);
		ImGui::Text("Buttons Pressed");
		int Count = 0;
		for (auto& jj : Buttons)
		{
			
			ImGui::Text(jj.c_str());
			ImGui::SameLine();
			ImGui::Text((GLFW_PRESS == Bttns[Count])?(" Pressed for %.3f(s)"):(" Released"),BttnHold[Count].Time);
			if (BttnHold[Count].D_Pressed)
			{
				ImGui::SameLine();
				ImGui::Text("double Pressed");
			}
			Count++;
		}
		
		ImGui::SliderFloat("Axis Rate X", &R_cntr_Ac[0], 0, 100);
		ImGui::SliderFloat("Axis Rate Y", &R_cntr_Ac[1], 0, 100);
	}
public:
	Input(int NewID)
	{
		JoyStickID = NewID;
	}
	void UpdateVals(float Dt)
	{
		Connected = (glfwJoystickPresent(JoyStickID) == GLFW_TRUE);
		if (Connected)
		{
			Bttns = glfwGetJoystickButtons(JoyStickID, &BttnAmount);
			if (BttnHold.size() == 0)
			{
				for (int jj = 0; jj < BttnAmount; jj++)
				{
					BttnHold.push_back({0.f,0.f,false});
				}
			}
			else
			{
				int Count = 0;
				for (auto& ii : BttnHold)
				{
					if (GLFW_PRESS == Bttns[Count])
					{	
						if (ii.Prev < 0.15f && ii.Prev != 0.f)
						{
							ii.D_Pressed = true;
						}
						else
						{
							ii.D_Pressed = false;
						}
						ii.Prev = 0.f;
						ii.Time += Dt;
					}
					else {
						if (!(ii.Prev >= 1.f))
						{
							ii.Prev += Dt;
						}
						ii.Time = 0;
					}
					Count++;
				}
			}
			Axis = glfwGetJoystickAxes(JoyStickID, &Amount);
		}
	}
	void RenderBox()
	{
		ImGui::Begin("Controller information");
		if (Connected)
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
		return Connected;
	}
	//Analog Values found
	std::vector<float> GetLeftAnalogVals()
	{
		return {Axis[0],Axis[1]};
	}
	std::vector<float> GetRightAnalogVals()
	{
		return { Axis[2],Axis[3] };
	}
	std::vector<float> GetAxisRate()
	{
		return this->R_cntr_Ac;
	}
	float GetLeftTrigVal()
	{	
		return Axis[4];
	}
	float GetRightTrigVal()
	{
		return Axis[5];
	}
	//-----Buttons Pressed
	//-- Face Button
	bool AButtonPressed()
	{
		return GLFW_PRESS == Bttns[0];
	}
	bool BButtonPressed()
	{
		return GLFW_PRESS == Bttns[1];
	}
	bool XButtonPressed()
	{
		return GLFW_PRESS == Bttns[2];
	}
	bool YButtonPressed()
	{
		return GLFW_PRESS == Bttns[3];
	}
	//-- Bumpers
	bool LeftBumperPressed()
	{
		return GLFW_PRESS == Bttns[4];
	}
	bool RightBumperPressed()
	{
		return GLFW_PRESS == Bttns[5];
	}
	//-- Middle Buttons
	bool ViewButtonPressed()
	{
		return GLFW_PRESS == Bttns[6];
	}
	bool MenuButtonPressed()
	{
		return GLFW_PRESS == Bttns[7];
	}
	//-- Analog Clicks
	bool LeftClickPressed()
	{
		return GLFW_PRESS == Bttns[8];
	}
	bool RightClickPressed()
	{
		return GLFW_PRESS == Bttns[9];
	}
	//-- D-Pad
	bool UpPressed()
	{
		return GLFW_PRESS == Bttns[10];
	}
	bool RightPressed()
	{
		return GLFW_PRESS == Bttns[11];
	}
	bool DownPressed()
	{
		return GLFW_PRESS == Bttns[12];
	}
	bool LeftPressed()
	{
		return GLFW_PRESS == Bttns[13];
	}
};