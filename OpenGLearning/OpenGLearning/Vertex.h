#pragma once
#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
};
struct AnimVertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::ivec3 MatId;
	glm::vec3 Weights;
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
private:
	glm::vec3 Convert(glm::vec3 Rot)
	{
		Rot = Rot / 180.f * glm::pi<float>();
		return Rot;
	}
public:
	glm::vec3 Position;
	glm::vec3 Origin;
	glm::quat Rotation;
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
		this->Rotation = this->Convert(Rot);		
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
struct Joints
{
private:
	
public:
	glm::vec3 Offset;	
	glm::quat Rotation;
	
};
struct QuatParts
{
	float Angle;
	glm::vec3 UnitVec;
	QuatParts(glm::quat QuatPart)
	{
		this->Angle = 2 * glm::acos(QuatPart.w) / glm::pi<float>() * 180.f;
		float AngRad = this->Angle / 180.f * glm::pi<float>();
		float SinPart = glm::sin(AngRad/2);
		this->UnitVec.x = QuatPart.x / SinPart;
		this->UnitVec.y = QuatPart.y / SinPart;
		this->UnitVec.z = QuatPart.z / SinPart;
	}
};
class Nodes
{
	Nodes* Parent;
	glm::vec3 Position;
	glm::quat Rot;
	glm::vec3 Scale;
	glm::vec3 Origin;
	glm::mat4 Matrix;
	glm::vec3 RelPos;
	int ParentId;
	int MeshId;
	glm::vec3 Convert(glm::vec3 Rot)
	{
		Rot = Rot / 180.f * glm::pi<float>();
		return Rot;
	}
public:
	Nodes(Nodes* InitParent,
		glm::vec3 InitPosition, glm::vec3 Origin, glm::quat InitRotation, glm::vec3 InitScale, int InitParentID, int InitMeshId)
		:Parent(InitParent), Position(InitPosition), Scale(InitScale), Origin(Origin),
		Matrix(glm::mat4(1.f))
	{
		this->Origin = this->Position;
		this->RelPos = this->Position - this->Origin;
		this->MeshId = InitMeshId;
		this->ParentId = InitParentID;
		this->Matrix = glm::translate(this->Matrix, this->Origin);
		this->Rot = InitRotation;
		Matrix *= glm::mat4_cast(this->Rot);		
		this->Matrix = glm::translate(this->Matrix, this->RelPos);
		this->Matrix = glm::scale(this->Matrix, this->Scale);
		
	}
	//Get Items
	glm::mat4 GetFinalMat4()
	{
		if (this->Parent)
		{
			return this->Parent->GetFinalMat4() * this->Matrix;
		}
		else
		{
			return this->Matrix;
		}
	}
	glm::vec3 GetOrigin()
	{
		return this->Origin;
	}
	glm::vec3 GetPosition()
	{
		return this->RelPos + this->Origin;
	}
	glm::vec3 GetRelPos()
	{
		return this->RelPos;
	}
	glm::quat GetRotation()
	{
		return this->Rot;
	}
	glm::vec3 GetScale()
	{
		return this->Scale;
	}
	std::string GetParentsId()
	{
		return std::to_string(this->ParentId);
	}
	int GetMeshId()
	{
		return this->MeshId;
	}
	//Update Matrix
	void UpdateMatrix()
	{
		this->Matrix = glm::mat4(1.f);
		this->Matrix = glm::translate(this->Matrix, this->Origin);
		glm::mat4 Temps = glm::mat4_cast(this->Rot);
		Matrix *= Temps;
		this->Matrix = glm::translate(this->Matrix, this->RelPos);
		this->Matrix = glm::scale(this->Matrix, this->Scale);
	}
	//Set Items
	void SetParent(Nodes* NewParent)
	{
		this->Parent = NewParent;
	}
	void SetOrigin(const glm::vec3 origin)
	{
		this->Origin = origin;
	}
	void SetRotation(const QuatParts Parts)
	{
		float RotAngle = Parts.Angle * glm::pi<float>() / 180.f;
		this->Rot.w = glm::cos(RotAngle / 2);
		this->Rot.x = Parts.UnitVec.x * glm::sin(RotAngle / 2);
		this->Rot.y = Parts.UnitVec.y * glm::sin(RotAngle / 2);
		this->Rot.z = Parts.UnitVec.z * glm::sin(RotAngle / 2);

	}
	void SetScale(const glm::vec3 setScale)
	{
		this->Scale = setScale;
	}
	void SetRelPos(const glm::vec3 SetRePos)
	{
		this->RelPos = SetRePos;
	}
	//Modifiers
	void Move(glm::vec3 Pos)
	{
		this->Position += Pos;
	}
	void ScaleUp(glm::vec3 Scale)
	{
		this->Scale += Scale;
	}
};
