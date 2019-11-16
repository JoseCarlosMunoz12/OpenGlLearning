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
	int ParentId;
	int OwnId;
	int MeshId;
	std::vector<int> TextsId;
	MeshsArtifacts()
	{
		this->Origin = glm::vec3(0.f);
		this->Scale = glm::vec3(1.f);
		this->Position = glm::vec3(0.f);
		this->Rotation = glm::vec3(0.f);
		this->MeshId = 0;
		this->ParentId = 0;
		this->OwnId = 0;
	}
	MeshsArtifacts(glm::vec3 Pos, glm::vec3 Origin, glm::vec3 Rot, glm::vec3 Scale,
		int Mesh,int Parent,int OwnIdInit,std::vector<int>Textures)
	{
		this->Origin =  Origin;
		this->Scale = Scale;
		this->Position = Pos;
		this->Rotation = Rot;
		this->MeshId = Mesh;
		this->ParentId = Parent;
		this->TextsId = Textures;
		this->OwnId = OwnIdInit;
	}
};
struct M_To_Make
{
	std::string NewName;
	glm::vec3 NewPos;
	glm::vec3 NewRot;
	std::vector<int> TexId;
	std::vector<int> MeshId;
	std::vector<MeshsArtifacts> NodesInf;
};