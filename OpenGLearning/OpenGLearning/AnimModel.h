#pragma once
#include "Vertex.h"
#include "SkelAnimat.h"
#include "AnimMesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include <map>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <vector>

struct AnimMeshArtifacts
{
	std::string Name;
	std::string Parent;
	std::vector<Frames*> AllFrames;
};
class AnimModel
{
private:
	StdMat* AnimMat;
	AnimMesh* meshes;
	std::vector<GeneralTextInfo*> Tex;
	std::vector<Nodes*> TreeNodes;
	std::vector<int> TextToUse;
	std::map<std::string, SkelAn*> Skeleton;
	glm::vec3 Origin;
	glm::vec3 RelPos;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	glm::mat4 FinalMatrix;
	std::string Name;
	float TimeLength;
	float TimePass = 0;
	void MakeSkeleton(std::vector<AnimMeshArtifacts> Inits)
	{
		for (auto& ii : Inits)
		{
			Skeleton[ii.Name] = new SkelAn(ii.AllFrames,ii.Parent);
		}
	}
	glm::vec3 Convert(glm::vec3 Rot)
	{
		Rot = Rot / 180.f * glm::pi<float>();
	}
	void UpdateMatrix()
	{
		this->FinalMatrix = glm::mat4(1.f);
		this->FinalMatrix = glm::translate(this->FinalMatrix, this->Origin);
		glm::quat Temp = glm::quat(this->Convert(this->Rotation));
		glm::mat4 Temps = glm::mat4_cast(Temp);
		this->FinalMatrix *= Temps;
		this->FinalMatrix = glm::translate(this->FinalMatrix, this->RelPos);
		this->FinalMatrix = glm::scale(this->FinalMatrix, this->Scale);
	}
	std::vector<glm::mat4> UpdateTime(float TimePass)
	{
		std::vector<glm::mat4> TempMats;
		if (TimePass > TimeLength)
		{
			this->TimePass = 0;
		}
		else {
			this->TimePass += TimePass;
		}
		for (auto& Bone : Skeleton)
		{
			TempMats.push_back(Bone.second->GetCurMat(this->Skeleton, this->TimePass));
		}
		return TempMats;
	}
	std::vector<glm::mat4> GetCurMat()
	{
		std::vector<glm::mat4> TempMats;
		for (auto& Bone : Skeleton)
		{
			TempMats.push_back(Bone.second->GetCurMat(this->Skeleton, this->TimePass));
		}
		return TempMats;
	}
public:
	AnimModel(std::string ModName, glm::vec3 InitPos,
		StdMat* material,
		std::vector<GeneralTextInfo*> OrTexSpec,
		AnimMesh* AnimMeshToUse,
		std::vector<AnimMeshArtifacts> Inits,
		glm::vec3 InitOr = glm::vec3(0.f), glm::vec3 InitRot = glm::vec3(0.f))
		:Name(ModName), AnimMat(material)
	{
		this->Origin = InitPos;
		this->RelPos = this->Origin - InitOr;
		this->Tex = OrTexSpec;
		this->meshes = AnimMeshToUse;
		this->MakeSkeleton(Inits);
		this->UpdateMatrix();
	}
	~AnimModel()
	{

	}
	//Setters
	void SetOrigin(glm::vec3 NewOrigin)
	{
		this->Origin = NewOrigin;
	}
	void SetRelPos(glm::vec3 NewRelPos)
	{
		this->RelPos = NewRelPos;
	}
	void SetRot(glm::vec3 NewRot)
	{
		this->Rotation = NewRot;
	}
	void SetScale(glm::vec3 NewScale)
	{
		this->Scale = NewScale;
	}
	//Getters
	glm::vec3 GetPosition()
	{
		return this->RelPos + this->Origin;
	}
	glm::vec3 Origin()
	{
		return this->Origin;
	}
	glm::vec3 GetRelPos()
	{
		return this->RelPos;
	}
	glm::vec3 GetRot()
	{
		return this->Rotation;
	}
	glm::vec3 GetScale()
	{
		return this->Scale;
	}
	//Render
	void Render(float TimePass, std::vector<Shader*>shader, std::vector<glm::mat4> LightMatrix)
	{
		this->UpdateMatrix();
		int TempShaderId = this->AnimMat->GetShaderId();
		this->AnimMat->SendToShader(shader, LightMatrix);
		int Num = 0;

		for (auto& ii : this->TextToUse)
		{
			this->Tex[ii]->Bind(Num);
			Num++;
		}
		meshes->Render(glm::mat4(1.f), shader[TempShaderId], this->UpdateTime(TimePass));

	}
	//Other
	//Shadow Renderer
	void ShadowRender(float PassTime, Shader* ShadowShader)
	{
		this->UpdateMatrix();
		ShadowShader->use();
		this->meshes->Render(this->FinalMatrix, ShadowShader, this->GetCurMat());
	}
	//Get other general information
	std::vector<GeneralTextInfo*> GetTextures()
	{
		return this->Tex;
	}
	std::vector<int> GetTexId()
	{
		return this->TextToUse;
	}
	StdMat* GetStdMat()
	{
		return this->AnimMat;
	}
	std::string GetName()
	{
		return this->Name;
	}
	int GetShaderId()
	{
		return this->AnimMat->GetShaderId();
	}
};