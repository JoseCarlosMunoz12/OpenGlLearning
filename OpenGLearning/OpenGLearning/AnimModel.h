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
	std::vector<SkelAn*> AnimBones;
	glm::vec3 Origin;
	glm::vec3 RelPos;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	std::string Name;
	float TimeLength;
	float TimePass = 0;
	void MakeSkeleton()
	{

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
		for (auto& Bone :Skeleton)
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
		std::vector<MeshsArtifacts> Inits,
		glm::vec3 InitOr = glm::vec3(0.f), glm::vec3 InitRot = glm::vec3(0.f))
		:Name(ModName),AnimMat(material)
	{
		this->Origin = InitPos;
		this->RelPos = this->Origin - InitOr;
		this->Tex = OrTexSpec;
		this->meshes = AnimMeshToUse;
	}
	~AnimModel()
	{

	}
	//Format
	//Setters
	//Getters
	//Render
	void Render(float TimePass,std::vector<Shader*>shader)
	{
		meshes->Render(glm::mat4(1.f), shader[0], this->UpdateTime(TimePass));
	}
	//Other
	//Shadow Renderer
	//Get other general information
};