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
struct Quaterions {
	float X;
	float Y;
	float Z;
	float W;
	Quaterions(float InitX, float InitY, float InitZ, float InitW)
	{
		this->X = InitX;
		this->Y = InitY;
		this->Z = InitZ;
		this->W = InitW;
	}
	Quaterions(float Val)
	{
		this->X = Val;
		this->Y = Val;
		this->Z = Val;
		this->W = Val;
	}
	Quaterions(glm::mat4 Mat)
	{
		float Diagnoal = Mat[0].x + Mat[1].y + Mat[2].z;
		if (Diagnoal > 0)
		{
			float W4 = glm::sqrt(Diagnoal + 1.f) * 2.f;
			this->W = W4 / 4.f;
			this->X = (Mat[2].z - Mat[1].y) / W4;
			this->Y = (Mat[0].z - Mat[2].x) / W4;
			this->Z = (Mat[1].x - Mat[0].y);
		}else if ((Mat[0].x > Mat[1].y) && (Mat[0].x > Mat[2].z)){

		}else if (Mat[1].y > Mat[2].z) {

		}else {

		}

	}
	glm::mat4 ToMat4()
	{

	}
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
	}
	MeshsArtifacts(glm::vec3 Pos, glm::vec3 Origin, glm::vec3 Rot, glm::vec3 Scale,
		int Mesh,int Parent,std::vector<int>Textures)
	{
		this->Origin =  Origin;
		this->Scale = Scale;
		this->Position = Pos;
		this->Rotation = Rot;
		this->MeshId = Mesh;
		this->ParentId = Parent;
		this->TextsId = Textures;
	}
};
struct NodesId
{
	glm::vec3 Pos;
	glm::vec3 Rot;
	glm::vec3 Scale;
	glm::vec3 Origin;
	int ParentId;
	int MeshId;
	std::vector<int> TexId;
};
struct MdlToMake
{
	std::string NewName;
	glm::vec3 NewPos;
	glm::vec3 NewRot;
	glm::vec3 NewScale;
	int MatId;
	std::vector<std::string> MeshesName;
	std::vector<std::string> TexNames;
	std::vector<NodesId> NodesInf;
};