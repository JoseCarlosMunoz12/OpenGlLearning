#pragma once
#include "Vertex.h"
#include "SkelAnimat.h"
#include "AnimMesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "CoatlPhysicsEngine.h"
#include <map>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <vector>
#include <Chrono>
#include <thread>
#include <mutex>


class AnimModel
{
private:
	struct OrdStruct
	{
		std::string Bone;
		bool Active;
	};
	int BodID;
	StdMat* AnimMat;	
	std::vector<AnimMesh*> meshes;
	std::vector<GeneralTextInfo*> Tex;
	std::vector<Nodes*> TreeNodes;
	std::vector<int> TextToUse;
	std::map<std::string, SkelAn*> BaseSKel;
	std::map<std::string, Animation*> Animations;
	std::vector<OrdStruct> OrRend;
	std::vector<glm::mat4> AllMats;
	std::vector<std::string> Blend;
	std::string Name;
	std::string CurAnim;
	std::string NewAnim;
	std::weak_ptr<CPE::Bodies> ColBody;
	float TimeLength;
	float TransLength;
	int AnimChosen = 0;
	float TimePass = 0;
	float TransTimePass = 0;
	float BiasVal = 0.f;
	void MakeAnimationInfo(std::vector<AnimArti> AnimInits)
	{
		std::map<std::string, SkelAn*> BaseMap;
		this->CurAnim = "";
		for (auto& ii : AnimInits)
		{
			std::vector<std::string> TempOrder;
			std::map<std::string, SkelAn*> TempMap;
			for (auto& jj : ii.Inits)
			{
				TempMap[jj.Name] = new SkelAn(jj.AllFrames, jj.Parent, jj.TransMat, jj.OffsetMat, jj.InitOffset, jj.InitQuat, jj.InitScale);
				TempOrder.push_back(jj.Name);
				Frames* TempFrame = new Frames(0.f, { jj.InitOffset,jj.InitScale,QuatParts(0.f) });
				BaseMap[jj.Name] = new SkelAn({ TempFrame }, jj.Parent, jj.TransMat, jj.OffsetMat, jj.InitOffset, jj.InitQuat, jj.InitScale);
			}
			this->Animations[ii.Name] = new Animation(ii.Name, TempMap, TempOrder, ii.TimeLength, ii.Inv);
			this->BaseSKel = BaseMap;
			BaseMap.clear();
		}
		if (AnimInits.size() != 1)
		{
			this->CurAnim = AnimInits[1].Name;
		}
		std::vector<std::string> OrdRend = this->Animations[this->CurAnim]->GetOrder();
		this->TimeLength = this->Animations[this->CurAnim]->GetTimeLength();
		for (auto& jj : OrdRend)
		{
			OrRend.push_back({ jj,false });
		}
	}
	void MakeNodes(glm::vec3 Pos, std::vector<MeshsArtifacts>Inits)
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
	//Animation Functions
	void UpdateTime(float TimePass, bool ContTime = true)
	{
		if (ContTime)
		{
			this->TimePass += TimePass;
			if ((this->TimePass >= this->TimeLength) || (TimePass >= this->TimeLength))
			{
				this->TimePass = 0;
			}
		}
		else
		{
			if (TimePass >= this->TimeLength)
			{
				this->TimePass = this->TimeLength;
			}
			else
			{
				this->TimePass = TimePass;
			}
		}
		int Count = 0;
		for (auto& Bone : OrRend)
		{
			this->AllMats[Count] = this->Animations[this->CurAnim]->GetCurMat(Bone.Bone, this->TimePass);
			Count++;
		}
	}
	void UpdateMats()
	{
		int Count = 0;
		for (auto& Bone : OrRend)
		{
			this->AllMats[Count] = this->Animations[this->CurAnim]->GetMat(Bone.Bone, Bone.Active);
			Count++;
		}
	}
	void GetCurMat()
	{
		for (auto& Bone : OrRend)
		{
			this->AllMats.push_back(this->Animations[this->CurAnim]->GetMat(Bone.Bone, false));
		}
	}
	void GetBlendMats(float TimePass, std::vector<std::string> Anims, bool ContTime = true)
	{
		int Count = 0;
		if (ContTime)
		{
			this->TimePass += TimePass;
			if ((this->TimePass >= this->TimeLength) || (TimePass >= this->TimeLength))
			{
				this->TimePass = 0;
			}
		}
		else
		{
			if (TimePass >= this->TimeLength)
			{
				this->TimePass = this->TimeLength;
			}
			else
			{
				this->TimePass = TimePass;
			}
		}
		glm::mat4 Mat0;
		glm::mat4 Mat1;
		for (auto& Bone : OrRend)
		{
			Mat0 = this->Animations[Anims[0]]->GetCurMat(Bone.Bone, this->TimePass);
			Mat1 = this->Animations[Anims[1]]->GetCurMat(Bone.Bone, this->TimePass);
			this->AllMats[Count] = Mat0 * (1 - this->BiasVal) + Mat1 * this->BiasVal;
			Count++;
		}
	}
	void GetTransitionMats(float TimePass, std::string TransAnim, bool& AnimChange, bool ContTime = true)
	{
		if (ContTime)
		{
			this->TransTimePass += TimePass;
			if (this->TransTimePass >= this->TransLength)
			{
				this->TransTimePass = 0.f;
				this->CurAnim = TransAnim;
				this->TimePass = 0.f;
				AnimChange = false;
				return;
			}
		}
		else
		{
			if (TimePass >= this->TransLength)
			{
				this->TransTimePass = TransLength;
			}
			else
			{
				this->TransTimePass = TimePass;
			}
		}
		glm::mat4 Mat0;
		glm::mat4 Mat1;
		float Ratio;
		if( this->TransLength == 0)
		{
			Ratio = 1;
		}
		else
		{
			Ratio = this->TransTimePass / this->TransLength;
		}
		int Count = 0;
		for (auto& Bone : OrRend)
		{
			Mat0 = this->Animations[this->CurAnim]->GetCurMat(Bone.Bone, this->TimePass);
			Mat1 = this->Animations[TransAnim]->GetCurMat(Bone.Bone, 0.f);
			this->AllMats[Count] = Mat0 * (1 - Ratio) +  Mat1 * Ratio;
			Count++;
		}
	}
public:
	AnimModel(std::string ModName, glm::vec3 InitPos,
		StdMat* material,
		std::vector<GeneralTextInfo*> OrTexSpec,
		AnimMesh* AnimMeshToUse,
		std::vector<MeshsArtifacts>M_Inits,
		glm::vec3 InitOr = glm::vec3(0.f), glm::vec3 InitRot = glm::vec3(0.f))
		:Name(ModName), AnimMat(material),TransLength(1.f)
	{
		this->Tex = OrTexSpec;
		this->meshes.push_back(AnimMeshToUse);
		this->MakeAnimationInfo(AnimMeshToUse->GetInits());
		this->MakeNodes(InitPos, M_Inits);
		this->GetCurMat();
		this->Blend = { "","" };
	}
	~AnimModel()
	{

	}
	void UpdateCollisions()
	{
		if (!this->ColBody.expired())
		{
			glm::vec3 ColPos = this->ColBody.lock()->GetPos();
			this->SetOrigin(ColPos);
		}
	}
	void SetColBody(std::shared_ptr<CPE::Bodies> NewBod)
	{
		this->ColBody = NewBod;
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
	void SetBlends(std::vector<std::string> NewBlend)
	{
		this->Blend = NewBlend;
	}
	void SetBiasBlend(float NewBias)
	{
		this->BiasVal = NewBias;
	}
	void SetTransTime(float NewTime)
	{
		this->TransLength = NewTime;
	}
	void SetNewTransAnm(std::string NewAnim)
	{
		this->NewAnim = NewAnim;
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
	std::vector<std::string> GetBlends()
	{
		return this->Blend;
	}
	float GetBiasBlend()
	{
		return this->BiasVal;
	}
	float GetTransTime()
	{
		return this->TransLength;
	}
	float GetTransTimePass()
	{
		return this->TransTimePass;
	}
	std::string GetCurNewAnim()
	{
		return this->NewAnim;
	}
	//Render
	void Render(float TimePass,
		std::vector<Shader*>shader, std::vector<glm::mat4> LightMatrix ,
		bool TimeDep, bool Slider,bool& Translate , bool BlendAnim = false)
	{
		for (auto& ii : this->TreeNodes)
		{
			ii->UpdateMatrix();
		}
		if (TimeDep)
		{
			if (BlendAnim)
			{
				this->GetBlendMats(TimePass, this->Blend, !Slider);
			}
			else if (Translate)
			{
				this->GetTransitionMats(TimePass, this->NewAnim,Translate,!Slider);
			}
			else
			{
				this->UpdateTime(TimePass,!Slider);
			}
		}
		else
		{
			this->UpdateMats();
		}		
		int TempShaderId = this->AnimMat->GetShaderId();
		this->AnimMat->SendToShader(shader, LightMatrix);
		int Num = 0;
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
		std::vector<std::string> OrdRend;
		for (auto& jj : this->OrRend)
		{
			OrdRend.push_back(jj.Bone);
		}
		this->Animations[NewAnim] = new Animation(NewAnim,BaseSKel, OrdRend,TimeLength,this->Animations[""]->GetInv());
	}
	std::string DeleteAnimation(std::string AnimToDelete)
	{
		this->Animations.erase(AnimToDelete);
		std::vector<std::string> Temp = this->AllAnimations();
		if (Temp.size() < 1)
		{
			this->CurAnim = this->AllAnimations()[1];
			return this->CurAnim;
		}
		else
		{
			this->CurAnim = this->AllAnimations()[0];
			return this->CurAnim;
		}
	}
	void ChangeBoneBool(std::string Bone)
	{
		std::vector<OrdStruct>::iterator I = std::find_if(OrRend.begin(),OrRend.end(),
												[Bone](const OrdStruct& Vi){return Vi.Bone == Bone;});
		I->Active = true;
	}
	void SetBodId(int NewID)
	{
		this->BodID = NewID;
	}
	int GetBodId()
	{
		return this->BodID;
	}
};