#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include <iostream>

class Model
{
private:
	Material* material;
	std::vector<Texture*> Tex;
	std::vector<Mesh*> meshes;
	glm::vec3 Position;
	void updateUniform()
	{

	}
	void updateCollision(glm::vec3 Collisiontest)
	{
		meshes[0]->Update(Collisiontest);
	}
public:
	Model(glm::vec3 position, Material* material, Texture* orTexDif, Texture* orTexSpec,std::vector<Mesh*> meshes)
	{
		this->Position = position;
		this->material = material;
		this->Tex.push_back(orTexDif);
		this->Tex.push_back(orTexSpec);
		for (auto* i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}
		for (auto& i : this->meshes)
		{
			i->move(this->Position);
			i->setOrigin(this->Position);
		}
	}
	Model(glm::vec3 position, Material* material,
		std::vector<Texture*> orTexSpec, std::vector<Mesh*> meshes)
	{
		this->Position = position;
		this->material = material;
		this->Tex = orTexSpec;
		for (auto* i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}
		for (auto& i : this->meshes)
		{
			i->move(this->Position);
			i->setOrigin(this->Position);
		}
	}
	Model(glm::vec3 position, Material* material, Texture* orTexDif, Texture* orTexSpec, Mesh* meshesUse)
	{
		this->Position = position;
		this->material = material;
		this->Tex.push_back(orTexDif);
		this->Tex.push_back(orTexSpec);
		this->meshes.push_back(new Mesh(*meshesUse));
		for (auto& i : this->meshes)
		{
			i->move(this->Position);
			i->setOrigin(this->Position);
		}
	}
	Model(glm::vec3 position, Material* material, std::vector<Texture*> orTexSpec, Mesh* meshesUse)
	{
		this->Position = position;
		this->material = material;
		this->Tex = orTexSpec;
		this->meshes.push_back(new Mesh(*meshesUse));
		for (auto& i : this->meshes)
		{
			i->move(this->Position);
			i->setOrigin(this->Position);
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
	void render(Shader* shader)
	{
		//Update Uniforms
		this->updateUniform();
		//Update Uniforms
		this->material->sendToShader(*shader);
		shader->use();
		int Num = 0;
		for (auto& i : Tex)
		{
			i->bind(Num);
			Num++;
		}

		for (auto& i : this->meshes)
		{
			i->render(shader);
		}
	}
	void renderManyTextures(Shader* shader)
	{
		//Update Uniforms
		this->updateUniform();
		//Update Uniforms
		this->material->sendManyTexToShader(*shader);
		shader->use();
		int Num = 0;
		for (auto& i : Tex)
		{
			i->bind(Num);
			Num++;
		}

		for (auto& i : this->meshes)
		{
			i->render(shader);
		}
	}
};

