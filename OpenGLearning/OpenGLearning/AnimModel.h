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
	std::vector<int> MeshToUse;
	std::vector<std::vector<int>> TextToUse;
	std::vector<SkelAn*> AnimBones;
	glm::vec3 Origin;
	glm::vec3 RelPos;	
	std::string Name;

public:
	AnimModel(std::string ModName, glm::vec3 InitPos,
		StdMat* material,
		std::vector<GeneralTextInfo*> OrTexSpec,
		std::vector<AnimMesh*> AnimMeshToUse, glm::vec3 InitOr = glm::vec3(0.f))
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
	//Other
	//Shadow Renderer
	//Get other general information
};