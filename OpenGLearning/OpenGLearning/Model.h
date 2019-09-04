#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Vertex.h"
#include <iostream>

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
		:Parent(InitParent), Position(InitPosition), Rotation(InitRotation), Scale(InitScale), Origin(Origin),
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
		this->Matrix = glm::translate(this->Matrix, this->Position);
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
	std::vector<int> MeshToUse;
	std::vector<std::vector<int>> TextToUse;
	glm::vec3 Position;
	const char* Name;
	void updateUniform()
	{

	}
	void updateCollision(glm::vec3 Collisiontest)
	{
		meshes[0]->Update(Collisiontest);
	}
	void MakeNodes(glm::vec3 Pos,std::vector<MeshsArtifacts> Inits)
	{
		int Count = 0;
		for (auto ii : Inits)
		{
			if (Count == 0)
				this->TreeNodes.push_back(new Nodes(NULL,
					Pos, ii.Origin, ii.Rotation, ii.Scale));
			else
				this->TreeNodes.push_back(new Nodes(this->TreeNodes[ii.ParentId],
					ii.Position, ii.Origin, ii.Rotation, ii.Scale));
			this->MeshToUse.push_back(ii.MeshId);
			this->TextToUse.push_back(ii.TextsId);
			Count++;
		}
	}
public:
	Model(const char* ModelName,
		glm::vec3 position,
		StdMat* material,std::vector<Texture*>orTexSpec,std::vector<Mesh*> MeshesToUse,
		std::vector<MeshsArtifacts> Inits,
		glm::vec3 InitRot = glm::vec3(0.f))
	{
		this->Position = position;
		this->TestMat = material;
		this->Tex = orTexSpec;
		this->Name = ModelName;
		this->meshes = MeshesToUse;
		this->MakeNodes(position, Inits);
		this->TreeNodes[0]->SetOrigin(this->Position);
		this->TreeNodes[0]->SetRotation(InitRot);
	}	
	Model(const char* ModelName,
		glm::vec3 position,
		StdMat* material, std::vector<Texture*>orTexSpec, Mesh* MeshUse, std::vector<MeshsArtifacts> Inits,
		glm::vec3 InitRot = glm::vec3(0.f))
	{
		this->Position = position;
		this->TestMat = material;
		this->Tex = orTexSpec;
		this->Name = ModelName;
		this->meshes.push_back(MeshUse);
		this->MakeNodes(position, Inits);
		this->TreeNodes[0]->SetOrigin(this->Position);
		this->TreeNodes[0]->SetRotation(InitRot);
	}
	~Model()
	{
	}
	//Format
	void rotate(const glm::vec3 rotation,int MeshId)
	{
		
		this->TreeNodes[MeshId]->Rotate(rotation);
		
	}
	void move(const glm::vec3 Move, int MeshId)
	{
		this->TreeNodes[MeshId]->Move(Move);
	}
	void scale(const glm::vec3 ReScale, int MeshId)
	{
		this->TreeNodes[MeshId]->ScaleUp(ReScale);
	}
	//Setters
	void SetRotation(const glm::vec3 rotation, int MeshId)
	{

		this->TreeNodes[MeshId]->SetRotation(rotation);

	}
	void SetPos(const glm::vec3 Move, int MeshId)
	{
		this->TreeNodes[MeshId]->SetPosition(Move);
	}
	void SetScale(const glm::vec3 ReScale, int MeshId)
	{
		this->TreeNodes[MeshId]->SetScale(ReScale);
	}
	//Getters
	glm::vec3 GetRotation(int MeshId)
	{
		return this->TreeNodes[MeshId]->GetRotation();
	}
	glm::vec3 GetPosition(int MeshId)
	{
		return this->TreeNodes[MeshId]->GetPosition();
	}
	glm::vec3 GetScale(int MeshId)
	{
		return this->TreeNodes[MeshId]->GetScale();
	}
	//Other
	void TestRender(std::vector<Shader*> shader)
	{
		for (auto& ii : this->TreeNodes)
		{
			ii->UpdateMatrix();
		}
		this->updateUniform();
		int TempShdrId = this->TestMat->GetShaderId();
		this->TestMat->sendToShader(shader);
		shader[TempShdrId]->use();
		Shader* T = shader[TempShdrId];
		int Num = 0;
		for (auto& ii : this->TreeNodes)
		{
			std::vector<int> Temp = this->TextToUse[Num];
			int Count = 0;
			for (auto jj : Temp)
			{
				this->Tex[jj]->bind(Count);
				Count++;
			}
			this->meshes[this->MeshToUse[Num]]->Render(ii->GetFinalMat4(), T);
			Num++;
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
	glm::vec3 GetPosition()
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
