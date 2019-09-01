#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include <iostream>
#include "Vertex.h"

class Nodes
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	glm::vec3 Origin;
	glm::mat4 Matrix;
public:
	Nodes* Parent;
	Nodes(Nodes* InitParent,
		glm::vec3 InitPosition, glm::vec3 Origin, glm::vec3 InitRotation, glm::vec3 InitScale)
		:Parent(InitParent),Position(InitPosition), Rotation(InitRotation), Scale(InitScale), Origin(Origin),
		Matrix(glm::mat4(1.f))
	{
		this->Matrix = glm::translate(this->Matrix, this->Origin);
		this->Matrix = glm::rotate(this->Matrix, glm::radians(this->Rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->Matrix = glm::rotate(this->Matrix, glm::radians(this->Rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->Matrix = glm::rotate(this->Matrix, glm::radians(this->Rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->Matrix = glm::translate(this->Matrix, this->Position - this->Origin);
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
	glm::vec3 GetPosition()
	{
		return this->Position;
	}
	glm::vec3 GetRotation()
	{
		return this->Rotation;
	}
	glm::vec3 GetScale()
	{
		return this->Scale;
	}
	int GetTotalparent()
	{
		if (this->Parent)
		{
			return 1 + this->Parent->GetTotalparent();
		}
		else
		{
			return 0;
		}
	}
	//Update Matrix
	void UpdateMatrix()
	{
		this->Matrix = glm::mat4(1.f);
		this->Matrix = glm::translate(this->Matrix, this->Origin);
		this->Matrix = glm::rotate(this->Matrix, glm::radians(this->Rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->Matrix = glm::rotate(this->Matrix, glm::radians(this->Rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->Matrix = glm::rotate(this->Matrix, glm::radians(this->Rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->Matrix = glm::translate(this->Matrix, this->Position - this->Origin);
		this->Matrix = glm::scale(this->Matrix, this->Scale);
	}
	//Set Items
	void SetParent(Nodes* NewParent)
	{
		this->Parent = NewParent;
	}
	void SetPosition(const glm::vec3 position)
	{
		this->Position = position;
	}
	void SetOrigin(const glm::vec3 origin)
	{
		this->Origin = origin;
	}
	void SetRotation(const glm::vec3 rotation)
	{
		this->Rotation = rotation;
	}
	void SetScale(const glm::vec3 setScale)
	{
		this->Scale = setScale;
	}
	//Modifiers
	void Move(glm::vec3 Pos)
	{
		this->Position += Pos;
	}
	void Rotate(glm::vec3 Rot)
	{
		this->Rotation += Rot;
	}
	void ScaleUp(glm::vec3 Scale)
	{
		this->Scale += Scale;
	}
};

class Model
{
private:
	StdMat* TestMat;
	std::vector<Texture*> Tex;
	std::vector<Mesh*> meshes;
	std::vector<Nodes*> TreeNodes;
	std::vector<int> MeshesId;
	glm::vec3 Position;
	const char* Name;
	void updateUniform()
	{

	}
	void updateCollision(glm::vec3 Collisiontest)
	{
		meshes[0]->Update(Collisiontest);
	}
public:
	Model(glm::vec3 position, StdMat* material,
		std::vector<Texture*> orTexSpec, Mesh* meshesUse,
		const char* ModelName)
	{
		this->Position = position;
		this->TestMat = material;
		this->Tex = orTexSpec;
		this->Name = ModelName;
		this->meshes.push_back(new Mesh(*meshesUse));
	
	}
	Model(glm::vec3 position, StdMat* material,
		std::vector<Texture*> orTexSpec, std::vector<Mesh*> meshesUse,
		std::vector<int> MeshParentsIndex,
		const char* ModelName)
	{
		this->Position = position;
		this->TestMat = material;
		this->Tex = orTexSpec;
		this->Name = ModelName;
		this->meshes = meshesUse;
		
	}
	Model(glm::vec3 position, StdMat* material,
		std::vector<Texture*> orTexSpec, std::vector<Mesh*> meshesUse,
		std::vector<int> MeshParentsIndex,std::vector<MeshsArtifacts> MeshArt,std::vector<int> NodesId,
		const char* ModelName)
	{
		this->Position = position;
		this->TestMat = material;
		this->Tex = orTexSpec;
		this->Name = ModelName;
		this->meshes = meshesUse;
		this->MeshesId = MeshParentsIndex;
		int Count = 0;
		for (auto& ii : NodesId)
		{
			if (Count == 0)
				this->TreeNodes.push_back(NULL);
			else
				this->TreeNodes

		}


	}
	~Model()
	{
		for (auto*& i: this->meshes)
		{
			delete i;
		}
	}
	void Rotate(const glm::vec3 rotation,int NodeId)
	{
		
		this->TreeNodes[NodeId]->Rotate(rotation);
		
	}
	void Move(const glm::vec3 move, int NodeId)
	{
		this->TreeNodes[NodeId]->Move(move);
	}
	void ReScale(const glm::vec3 scale, int NodeId)
	{
		this->TreeNodes[NodeId]->ScaleUp(scale);
	}
	void TestRender(std::vector<Shader*> shader)
	{
		this->updateUniform();
		int TempShdrId = this->TestMat->GetShaderId();
		this->TestMat->sendToShader(shader);
		shader[TempShdrId]->use();
		Shader* T = shader[TempShdrId];
		int Num = 0;
		for (auto& i : Tex)
		{
			i->bind(Num);
			Num++;
		}
		for (auto& i : this->meshes)
		{
			i->Render(glm::mat4(1.f),T);
		}
	}
	//Get the names for Tex, Mesh, Pos, and Material
	std::vector<Texture*> getTexture()
	{
		return this->Tex;
	}
	std::vector<Mesh*> GetMeshes()
	{
		return this->meshes;
	}
	glm::vec3 GetModelPosition()
	{
		return this->Position;
	}
	StdMat* GetStdMat()
	{
		return this->TestMat;
	}
	const char* GetName()
	{
		return this->Name;
	}
};
