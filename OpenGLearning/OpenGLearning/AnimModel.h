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


class AnimModel
{
private:
	StdMat* AnimMat;
	AnimMesh* meshes;
	std::vector<GeneralTextInfo*> Tex;
	std::vector<Nodes*> TreeNodes;
	std::vector<int> TextToUse;
	std::map<std::string, SkelAn*> Skeleton;
	std::string Name;
	float TimeLength;
	float TimePass = 0;
	void MakeSkeleton(std::vector<SkelArti> Inits)
	{		
		if (Inits.size() != 0)
		{
			for (auto& ii : Inits)
			{
				Skeleton[ii.Name] = new SkelAn(ii.AllFrames, ii.Parent);
			}
		}
		else
		{
		}
	}
	void SetUpInits()
	{

	}
	void MakeNodes( glm::vec3 Pos, std::vector<MeshsArtifacts>Inits)
	{
		int Count = 0;
		for (auto ii : Inits)
		{
			if (Count == 0)
			{
				this->TreeNodes.push_back(new Nodes(NULL,
					Pos, ii.Origin, ii.Rotation, ii.Scale, 0, ii.MeshId));
				Count++;
			}
			else
			{
				this->TreeNodes.push_back(new Nodes(this->TreeNodes[ii.ParentId],
					ii.Position, ii.Origin, ii.Rotation, ii.Scale, ii.ParentId, ii.MeshId));
			}
			//this->MeshToUse.push_back(ii.MeshId);
			this->TextToUse.push_back(ii.TextsId[0]);
		}

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
		std::vector<MeshsArtifacts>M_Inits,
		glm::vec3 InitOr = glm::vec3(0.f), glm::vec3 InitRot = glm::vec3(0.f))
		:Name(ModName), AnimMat(material)
	{
		this->Tex = OrTexSpec;
		this->meshes = AnimMeshToUse;		
		this->MakeSkeleton(this->meshes->GetInits());
		this->MakeNodes(InitPos, M_Inits);
	}
	~AnimModel()
	{

	}
	//Setters
	void SetOrigin(glm::vec3 NewOrigin)
	{
		this->TreeNodes[0]->SetOrigin(NewOrigin);
	}
	void SetRelPos(glm::vec3 NewRelPos)
	{
		this->TreeNodes[0]->SetRelPos(NewRelPos);
	}
	void SetRot(QuatParts NewRot)
	{
		this->TreeNodes[0]->SetRotation(NewRot);
	}
	void SetScale(glm::vec3 NewScale)
	{
		this->TreeNodes[0]->SetScale(NewScale);
	}
	//Getters
	glm::vec3 GetPosition()
	{
		return this->TreeNodes[0]->GetPosition();
	}
	glm::vec3 GetOrigin()
	{
		return this->TreeNodes[0]->GetOrigin();
	}
	glm::vec3 GetRelPos()
	{
		return this->TreeNodes[0]->GetRelPos();
	}
	QuatParts GetRot()
	{
		return this->TreeNodes[0]->GetRotation();
	}
	glm::vec3 GetScale()
	{
		return this->TreeNodes[0]->GetScale();
	}
	//Render
	void Render(float TimePass, std::vector<Shader*>shader, std::vector<glm::mat4> LightMatrix)
	{
		for (auto& ii : this->TreeNodes)
		{
			ii->UpdateMatrix();
		}
		int TempShaderId = this->AnimMat->GetShaderId();
		this->AnimMat->SendToShader(shader, LightMatrix);
		int Num = 0;
		shader[TempShaderId]->use();
		for (auto& ii : this->TextToUse)
		{
			this->Tex[ii]->Bind(Num);
			Num++;
		}
		meshes->Render(TreeNodes[0]->GetFinalMat4(), shader[TempShaderId], this->UpdateTime(TimePass));

	}
	//Other
	//Shadow Renderer
	void ShadowRender(float PassTime, Shader* ShadowShader)
	{

		for (auto& ii : this->TreeNodes)
		{
			ii->UpdateMatrix();
		}
		ShadowShader->use();
		this->meshes->Render(TreeNodes[0]->GetFinalMat4(), ShadowShader, this->GetCurMat());
	}
	//Get other general information
	std::vector<GeneralTextInfo*> GetTextures()
	{
		return this->Tex;
	}
	std::vector<int> GetTexId()
	{
		//return this->TextToUse;
		return {};
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