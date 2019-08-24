#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include <iostream>
class Model
{
private:
	StdMat* TestMat;
	std::vector<Texture*> Tex;
	std::vector<Mesh*> meshes;
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
		const char* ModelName, glm::vec3 InitRot = glm::vec3(0.f))
	{
		this->Position = position;
		this->TestMat = material;
		this->Tex = orTexSpec;
		this->Name = ModelName;
		this->meshes.push_back(new Mesh(*meshesUse));
		for (auto& i : this->meshes)
		{
			i->setOrigin(this->Position);
			i->move(this->Position);
			i->setRotation(InitRot);
		}
	}
	Model(glm::vec3 position, StdMat* material,
		std::vector<Texture*> orTexSpec, std::vector<Mesh*> meshesUse,
		const char* ModelName, glm::vec3 InitRot = glm::vec3(0.f))
	{
		this->Position = position;
		this->TestMat = material;
		this->Tex = orTexSpec;
		this->Name = ModelName;
		this->meshes = meshesUse;
		for (auto& i : this->meshes)
		{
			i->setOrigin(this->Position);
			i->move(this->Position);
			i->setRotation(InitRot);
		}
	}

	~Model()
	{
		for (auto*& i: this->meshes)
		{
			delete i;
		}
	}
	void rotate(const glm::vec3 rotation)
	{
		for (auto& i : this->meshes)
		{
			i->rotate(rotation);
		}
	}
	void update(glm::vec3 CollisionTest,glm::vec3 newPos)
	{
		this->updateCollision(CollisionTest);
		for (auto& ii : this->meshes)
		{
			ii->setPosition(newPos);
		}
	}
	void CheckForIntersection()
	{

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
			i->render(T);
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

class Nodes
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	glm::vec3 Origin;
	glm::mat4 Matrix;
	Nodes* Parent;
public:
	Nodes(std::string Name, Nodes* InitParent,
		glm::vec3 InitPosition,glm::vec3 Origin, glm::vec3 InitRotation, glm::vec3 InitScale)
		:Position(InitPosition),Rotation(InitRotation),Scale(InitScale),Origin(Origin),
		Matrix(glm::mat4(1.f))
	{
		this->Parent = InitParent;
		this->Matrix = glm::translate(this->Matrix, this->Origin);
		this->Matrix = glm::rotate(this->Matrix, glm::radians(this->Rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->Matrix = glm::rotate(this->Matrix, glm::radians(this->Rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->Matrix = glm::rotate(this->Matrix, glm::radians(this->Rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->Matrix = glm::translate(this->Matrix, this->Position - this->Origin);
		this->Matrix = glm::scale(this-> Matrix, this->Scale);
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
			return glm::mat4(1.f);
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
	//Set Itmes
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
