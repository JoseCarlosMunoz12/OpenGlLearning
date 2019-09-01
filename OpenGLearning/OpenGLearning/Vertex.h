#pragma once
#include <glm.hpp>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
};
struct MouseItems
{
	double X;
	double Y;
};
struct MousePositions
{
	double X;
	double Y;
	MousePositions operator = (MousePositions const& obj)
	{
		MousePositions res;
		X = obj.X;
		Y = obj.Y;
		res.X = obj.X;
		res.Y = obj.Y;
		return res;
	}
};
struct FrameBufferItems
{
	int Width;
	int Height;
};
struct ImGuiItems
{
	ImVec2 ScreenPos;
	ImVec2 WinSize;
};
struct MeshsArtifacts
{
	glm::vec3 Position;
	glm::vec3 Origin;
	glm::vec3 Rotation;
	glm::vec3 Scale;
};