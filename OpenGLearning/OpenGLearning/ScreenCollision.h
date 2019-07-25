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
		bool XSide = MousePos.X >= GItem.ScreenPos.x && MousePos.X <= GItem.WinSize.x;
		bool YSide = MousePos.Y >= GItem.ScreenPos.y && MousePos.Y <= GItem.WinSize.y;

		return YSide && XSide ;
	}
};