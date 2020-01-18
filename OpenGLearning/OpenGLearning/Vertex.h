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
	glm::ivec4 MatId;
	glm::vec4 Weights;
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
struct QuatParts
{
	float Angle;
	glm::vec3 UnitVec;
	QuatParts()
	{
		this->Angle = 0.f;
		this->UnitVec = glm::vec3(0.f,1.f,0.f);
	}
	QuatParts(float InitAngle)
	{
		this->Angle = InitAngle;
		this->UnitVec = glm::vec3(0.f, 1.f, 0.f);
	}
	QuatParts(float InitAngle, glm::vec3 InitVec)
	{
		this->Angle = InitAngle;
		this->UnitVec = glm::normalize(InitVec);
	}
	glm::quat GetQuat()
	{
		glm::quat Temp;
		float RadAngle = Angle / 180.f * glm::pi<float>();
		Temp.x = UnitVec.x * glm::sin(RadAngle / 2);
		Temp.y = UnitVec.y * glm::sin(RadAngle / 2);
		Temp.z = UnitVec.z * glm::sin(RadAngle / 2);
		Temp.w = glm::cos(RadAngle / 2);
		return Temp;

	}
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
		this->Rotation = glm::quat(glm::vec3(0.f));
		this->MeshId = 0;
		this->ParentId = 0;
	}
	MeshsArtifacts(glm::vec3 Pos, glm::vec3 Origin, glm::quat Rot, glm::vec3 Scale,
		int Mesh, int Parent, std::vector<int>Textures)
	{
		this->Origin = Origin;
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
	glm::quat Rot;
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
	glm::vec3 Offset;	
	glm::vec3 Scale;
	QuatParts Rot;
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
	glm::vec3 ToEulerAngles(glm::quat q)
	{
		glm::vec3 Angles;
		double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
		Angles.x = std::atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = 2 * (q.w * q.y - q.z * q.x);
		if (std::abs(sinp) >= 1)
			Angles.y = std::copysign(glm::pi<float>() / 2, sinp); // use 90 degrees if out of range
		else
			Angles.y = std::asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		Angles.z = std::atan2(siny_cosp, cosy_cosp);

		return Angles;
	}
public:
	Nodes(Nodes* InitParent,
		glm::vec3 InitPosition, glm::vec3 Origin, glm::quat InitRotation, glm::vec3 InitScale, int InitParentID, int InitMeshId)
		:Parent(InitParent), Position(InitPosition),Rot(InitRotation), Scale(InitScale), Origin(Origin),
		Matrix(glm::mat4(1.f))
	{
		this->Origin = this->Position;
		this->RelPos = this->Position - this->Origin;
		this->MeshId = InitMeshId;
		this->ParentId = InitParentID;
		this->Matrix = glm::translate(this->Matrix, this->Origin);
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
	glm::vec3 GetRotEuler()
	{
		return this->ToEulerAngles(this->Rot);
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
	void SetRotEuler(glm::vec3 NewAngles)
	{
		this->Rot = glm::quat(NewAngles);
	}
	void SetRotation(glm::quat Parts)
	{
		this->Rot = Parts;
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
