#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Vertex.h"
#include <iostream>
#include <string>

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
	std::string Name;
	void updateUniform()
	{

	}
	void updateCollision(glm::vec3 Collisiontest)
	{
	}
	void MakeNodes(glm::vec3 Pos,std::vector<MeshsArtifacts> Inits)
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
			this->MeshToUse.push_back(ii.MeshId);
			this->TextToUse.push_back(ii.TextsId);
		}
	}
public:
	Model(std::string ModelName,
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
	}	
	Model(std::string ModelName,
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
	}
	~Model()
	{
	}
	//Format
	void move(const glm::vec3 Move)
	{
		this->TreeNodes[0]->Move(Move);
	}
	void scale(const glm::vec3 ReScale)
	{
		this->TreeNodes[0]->ScaleUp(ReScale);
	}	
	//Getters
	glm::vec3 GetPosition()
	{
		return this->TreeNodes[0]->GetPosition();
	}
	glm::vec3 GetScale()
	{
		return this->TreeNodes[0]->GetScale();
	}
	glm::vec3 GetOrigin()
	{
		return this->TreeNodes[0]->GetOrigin();
	}
	QuatParts GetRotation()
	{
		return this->TreeNodes[0]->GetRotation();
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
	StdMat* GetStdMat()
	{		
		return this->TestMat;
	}
	std::string GetName()
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
