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
	bool ImGuiCollisionDetection(ImGuiItems GItem,
								 MousePositions MousePos)
	{
		bool XSide = MousePos.MousePosX >= GItem.ScreenPos.x && MousePos.MousePosX <= GItem.WinSize.x;
		bool YSide = MousePos.MousePosY >= GItem.ScreenPos.y && MousePos.MousePosY <= GItem.WinSize.y;

		return YSide && XSide ;
	}
};