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
	std::vector<AnimMesh*> meshes;
	std::vector<GeneralTextInfo*> Tex;
	std::vector<Nodes*> TreeNodes;
	std::vector<int> TextToUse;
	std::map<std::string, SkelAn*> BaseSKel;
	std::map<std::string, Animation*> Animations;
	std::vector<std::string> OrdRend;
	std::vector<glm::mat4> AllMats;
	std::string Name;
	std::string CurAnim;
	float TimeLength;
	int AnimChosen = 0;
	float TimePass = 0;
	void MakeAnimationInfo(std::vector<AnimArti> AnimInits)
	{
		std::map<std::string, SkelAn*> BaseMap;
		for (auto& ii : AnimInits)
		{			
			std::vector<std::string> TempOrder;
			std::map<std::string, SkelAn*> TempMap;
			for (auto& jj : ii.Inits)
			{
				TempMap[jj.Name] = new SkelAn(jj.AllFrames,jj.Parent,jj.InitOffset,jj.InitQuat,jj.InitScale);
				TempOrder.push_back(jj.Name);
				BaseMap[jj.Name] = new SkelAn({}, jj.Parent, jj.InitOffset, jj.InitQuat, jj.InitScale);
			}
			this->Animations[ii.Name] = new Animation(ii.Name,TempMap,TempOrder,ii.TimeLength);
			this->BaseSKel = BaseMap;
			BaseMap.clear();
		}
		this->CurAnim = "First";
		this->OrdRend = this->Animations[this->CurAnim]->GetOrder();
		this->TimeLength = this->Animations[this->CurAnim]->GetTimeLength();
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
	void ControlTime(float TimePass)
	{
		if (TimePass >= this->TimeLength)
		{
			this->TimePass = this->TimeLength;
		}
		else
		{
			this->TimePass = TimePass;
		}
		int Count = 0;
		for (auto& Bone : OrdRend)
		{
			this->AllMats[Count] = this->Animations[this->CurAnim]->GetCurMat(Bone, this->TimePass);			
			Count++;
		}
	}
	void UpdateTime(float TimePass)
	{
		this->TimePass += TimePass;
		if( (this->TimePass >= this->TimeLength) || ( TimePass >= this->TimeLength))
		{
			this->TimePass = 0;
		}
		int Count = 0;
		for (auto& Bone : OrdRend)
		{
			this->AllMats[Count] = this->Animations[this->CurAnim]->GetCurMat(Bone,this->TimePass);
			Count++;
		}
		
	}
	void UpdateMats()
	{
		int Count = 0;
		for (auto& Bone : OrdRend)
		{	
			this->AllMats[Count] = this->Animations[this->CurAnim]->GetMat(Bone);
			Count++;
		}	
	}
	void GetCurMat()
	{
		for (auto& Bone :OrdRend)
		{
			this->AllMats.push_back(this->Animations[this->CurAnim]->GetMat(Bone));
		}
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
		this->meshes.push_back(AnimMeshToUse);		
		this->MakeAnimationInfo(AnimMeshToUse->GetInits());
		this->MakeNodes(InitPos, M_Inits);
		this->GetCurMat();
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
	void SetTime(float NewTimeLength)
	{
		this->Animations[this->CurAnim]->EditTimeLength(NewTimeLength);
		this->TimeLength = this->Animations[this->CurAnim]->GetTimeLength();
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
	float GetAnimLength()
	{
		return this->TimeLength;
	}
	float GetTimePass()
	{
		return this->TimePass;
	}
	//Render
	void Render(float TimePass, std::vector<Shader*>shader, std::vector<glm::mat4> LightMatrix,bool TimeDep,bool Slider)
	{
		for (auto& ii : this->TreeNodes)
		{
			ii->UpdateMatrix();
		}
		if (TimeDep)
		{
			if (Slider)
			{
				this->ControlTime(TimePass);
			}
			else
			{
				this->UpdateTime(TimePass);
			}
		}
		else
		{
			this->UpdateMats();
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
		meshes[0]->Render(this->TreeNodes[0]->GetFinalMat4(), shader[TempShaderId],AllMats);

	}
	//Other
	//Shadow Renderer
	void ShadowRender(float PassTime, Shader* ShadowShader)
	{
		for (auto& ii : this->TreeNodes)
		{
			ii->UpdateMatrix();
		}
		this->UpdateMats();
		ShadowShader->use();
		this->meshes[0]->Render(TreeNodes[0]->GetFinalMat4(), ShadowShader,AllMats);
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
	std::map<std::string, SkelAn*> GetArt()
	{
		return this->Animations[this->CurAnim]->GetMap();
	}
	Animation* GetCurAnim()
	{
		return this->Animations[this->CurAnim];
	}
	std::vector<Animation*> GetAllAnimation()
	{
		std::vector<Animation*> TempAnim;
		for (auto& ii : this->Animations)
		{
			TempAnim.push_back(ii.second);
		}
		return TempAnim;
	}
	std::vector<Nodes*> GetNodesInfo()
	{
		return this->TreeNodes;
	}
	std::vector<AnimMesh*> GetMeshes()
	{
		return { this->meshes };
	}
	std::vector<std::string> AllAnimations()
	{
		std::vector < std::string > AllAnim;
		for (auto& ii : this->Animations)
		{
			AllAnim.push_back(ii.first);
		}
		return AllAnim;
	}
	std::string GetAnimName()
	{
		return this->CurAnim;
	}
	void ChangeAnim(std::string NewAnim)
	{
		this->CurAnim = NewAnim;
		this->TimeLength = this->Animations[this->CurAnim]->GetTimeLength();
	}
	void AddAnimation(std::string NewAnim,float TimeLength)
	{
		this->Animations[NewAnim] = new Animation(NewAnim,BaseSKel, this->OrdRend,TimeLength);
	}
	void DeleteAnimation(std::string AnimToDelete)
	{
		this->Animations.erase(AnimToDelete);
	}
};