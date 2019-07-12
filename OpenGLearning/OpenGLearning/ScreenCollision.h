#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Vertex.h"

class UICollision
{
public:
	UICollision()
	{

	}
	~UICollision()
	{

	}
	bool ImGuiCollisionDetection(ImVec2 ScreenPos, ImVec2 WinSize,
								 MousePositions MousePos)
	{
		bool XSide = MousePos.MousePosX >= ScreenPos.x && MousePos.MousePosX <= WinSize.x;
		bool YSide = MousePos.MousePosY >= ScreenPos.y && MousePos.MousePosY <= WinSize.y;

		return YSide && XSide ;
	}
};