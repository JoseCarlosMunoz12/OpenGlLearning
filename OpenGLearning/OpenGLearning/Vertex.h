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
		this->UnitVec = glm::vec3(0.f,0.f,1.f);
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
	QuatParts(glm::quat InitQuat)
	{
		if (InitQuat.w > 1.f)
			InitQuat = glm::normalize(InitQuat);
		float s = glm::sqrt(1 - InitQuat.w * InitQuat.w);
		this->Angle = 2.f * glm::acos(InitQuat.w) * 180.f / glm::pi<float>();;
		if (Angle == 0.f)
		{
			this->UnitVec = glm::vec3(0.f, 0.f, 1.f);
		}
		else
		{

			this->UnitVec = glm::vec3(InitQuat.x / s,
				InitQuat.y / s, InitQuat.z / s );
		}
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
	void Cout()
	{
		std::cout << this->Angle <<"\n";
		std::cout << this->UnitVec.x <<"*" << this->UnitVec.y << "*" << this->UnitVec.z << "\n";

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
	QuatParts Rotation;
	glm::vec3 Scale;
	int ParentId;
	int MeshId;
	std::vector<int> TextsId;
	MeshsArtifacts()
	{
		this->Origin = glm::vec3(0.f);
		this->Scale = glm::vec3(1.f);
		this->Position = glm::vec3(0.f);
		this->Rotation = QuatParts();
		this->MeshId = 0;
		this->ParentId = 0;
	}
	MeshsArtifacts(glm::vec3 Pos, glm::vec3 Origin, QuatParts Rot, glm::vec3 Scale,
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
	QuatParts Rot;
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
struct Bezier_Bais {
	float Point0;
	float Point1;
};
class Nodes
{
	Nodes* Parent;
	glm::vec3 Position;
	QuatParts Rot;
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
	glm::vec3 ConvertDeg(glm::vec3 Rot)
	{
		Rot = Rot * 180.f / glm::pi<float>();
		return Rot;
	}

	glm::vec3 ToEulerAngles(glm::quat q)
	{
		return glm::eulerAngles(q);
	}
public:
	Nodes(Nodes* InitParent,
		glm::vec3 InitPosition, glm::vec3 Origin, QuatParts InitRotation, glm::vec3 InitScale, int InitParentID, int InitMeshId)
		:Parent(InitParent), Position(InitPosition),Rot(InitRotation), Scale(InitScale), Origin(Origin),
		Matrix(glm::mat4(1.f))
	{
		this->Origin = this->Position;
		this->RelPos = this->Position - this->Origin;
		this->MeshId = InitMeshId;
		this->ParentId = InitParentID;
		this->Matrix = glm::translate(this->Matrix, this->Origin);
		Matrix *= glm::mat4_cast(this->Rot.GetQuat());		
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
	QuatParts GetRotation()
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
		glm::mat4 Temps = glm::mat4_cast(this->Rot.GetQuat());
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
	void SetRotation(QuatParts Parts)
	{
		this->Rot = Parts;
	}
	void SetScale(const glm::vec3 setScale)
	{
		this->Scale = setScale;
	}
	void SetRelPos(const glm::vec3 SetRePos)
	{
		glm::vec3 Diff = this->RelPos - SetRePos;
		this->RelPos = SetRePos;
		this->Origin += Diff;
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
	//relative modifiers and getters
	glm::vec3 GetWOrigin()
	{
		glm::mat4 W_M = glm::mat4(1.f);
		if (this->Parent)
			W_M = this->Parent->GetFinalMat4();
		return W_M * glm::vec4(this->GetOrigin(), 1.f);
	}
	glm::vec3 GetWRel()
	{
		glm::mat4 W_M = glm::mat4(1.f);
		if (this->Parent)
			W_M = this->Parent->GetFinalMat4();
		glm::vec3 Or = W_M * glm::vec4(this->GetOrigin(), 1.f);
		return Or + this->RelPos;
	}
	void SetWOrigin(glm::vec3 NewW_Pos)
	{
		glm::mat4 W_M = glm::mat4(1.f);
		if (this->Parent)
			W_M = this->Parent->GetFinalMat4();
		W_M = glm::inverse(W_M);
		this->Origin = W_M * glm::vec4(NewW_Pos,1.f);
	}
};
