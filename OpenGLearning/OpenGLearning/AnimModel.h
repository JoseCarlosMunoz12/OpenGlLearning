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
	StdMat* AnimTex;
	std::vector<AnimMesh*> meshes;
	std::vector<int> MeshToUse;
	std::vector<std::vector<int>> TextToUse;
	std::vector<SkelAn*> AnimBones;
	glm::vec3 Origin;
	glm::vec3 RelPos;
	std::string Name;

public:
	AnimModel()
	{

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