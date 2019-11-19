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
	int ParentId;
	int OwnId;
	int MeshId;
public:
	Nodes* Parent;
	Nodes(Nodes* InitParent,
		glm::vec3 InitPosition, glm::vec3 Origin, glm::vec3 InitRotation, glm::vec3 InitScale, int InitParentID, int InitOwnId,int InitMeshId)
		:Parent(InitParent), Position(InitPosition), Rotation(InitRotation), Scale(InitScale), Origin(Origin),
		Matrix(glm::mat4(1.f))
	{
		this->MeshId = InitMeshId;
		this->Rotation = InitRotation;
		this->ParentId = InitParentID;
		this->OwnId = InitOwnId;
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
	std::string GetParentsId()
	{
		if (this->Parent)
		{
			return this->Parent->GetParentsId() + "-" + std::to_string(this->ParentId);
		}
		else
		{
			return "M_N";
		}
	}
	std::string GetNodesId()
	{
		if (this->Parent)
		{
			return this->GetParentsId() + "-" + std::to_string(this->OwnId);
		}
		else
		{
			return "M_N";
		}
	}
	int GetMeshId()
	{
		return this->MeshId;
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
		if (this->Rotation.x > 360.f)
		{
			this->Rotation.x = 0.f;
		}
		else if (this->Rotation.x < 0.f)
		{
			this->Rotation.x < 360.f;
		}

		if (this->Rotation.y > 360.f)
		{
			this->Rotation.y = 0.f;
		}
		else if (this->Rotation.y < 0.f)
		{
			this->Rotation.y < 360.f;
		}

		if (this->Rotation.z > 360.f)
		{
			this->Rotation.z = 0.f;
		}
		else if (this->Rotation.z < 0.f)
		{
			this->Rotation.z < 360.f;
		}
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
	std::vector<GeneralTextInfo*> Tex;
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
					Pos, ii.Origin, ii.Rotation, ii.Scale,0,0,ii.MeshId));
			else
				this->TreeNodes.push_back(new Nodes(this->TreeNodes[ii.ParentId],
					ii.Position, ii.Origin, ii.Rotation, ii.Scale,ii.ParentId,ii.OwnId,ii.MeshId));
			this->MeshToUse.push_back(ii.MeshId);
			this->TextToUse.push_back(ii.TextsId);
			Count++;
		}
	}
public:
	Model(const char* ModelName,
		glm::vec3 position,
		StdMat* material,std::vector<GeneralTextInfo*>orTexSpec,std::vector<Mesh*> MeshesToUse,
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
		StdMat* material, std::vector<GeneralTextInfo*>orTexSpec, Mesh* MeshUse, std::vector<MeshsArtifacts> Inits,
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
	std::vector<Nodes*> GetNodesInfo()
	{
		return this->TreeNodes;
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
 		this->TestMat->SendToShader(shader);
		shader[TempShdrId]->use();
		Shader* T = shader[TempShdrId];
		int Num = 0;
		for (auto& ii : this->TreeNodes)
		{
			std::vector<int> Temp = this->TextToUse[Num];
			int Count = 0;
			for (auto jj : Temp)
			{
				this->Tex[jj]->Bind(Count);
				Count++;
			}
			this->meshes[this->MeshToUse[Num]]->Render(ii->GetFinalMat4(), T);
			Num++;
		}
	}

	void TestRender(std::vector<Shader*> shader,std::vector<glm::mat4> LightMatrix)
	{
		for (auto& ii : this->TreeNodes)
		{
			ii->UpdateMatrix();
		}
		this->updateUniform();
		int TempShdrId = this->TestMat->GetShaderId();
		this->TestMat->SendToShader(shader,LightMatrix);
		shader[TempShdrId]->use();
		Shader* T = shader[TempShdrId];
		int Num = 0;
		for (auto& ii : this->TreeNodes)
		{
			std::vector<int> Temp = this->TextToUse[Num];
			int Count = 0;
			for (auto jj : Temp)
			{
				this->Tex[jj]->Bind(Count);
				Count++;
			}
			this->meshes[this->MeshToUse[Num]]->Render(ii->GetFinalMat4(), T);
			Num++;
		}
	}
	//A Shadow Renderer
	void RenderShadow(Shader* ShadowShader)
	{
		for (auto& ii : this->TreeNodes)
		{
			ii->UpdateMatrix();			
		}
		ShadowShader->use();
		int Num = 0;
		for (auto& ii : this->TreeNodes)
		{
			std::vector<int> Temp = this->TextToUse[Num];
			this->meshes[this->MeshToUse[Num]]->Render(ii->GetFinalMat4(), ShadowShader);
			Num++;
		}

	}
	//Get the names for Tex, Mesh, Pos, and Material
	std::vector<GeneralTextInfo*> getTexture()
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
	std::vector<std::vector<int>> GetTexId()
	{
		return this->TextToUse;
	}
	int GetShaderId()
	{
		return this->TestMat->GetShaderId();
	}
};
