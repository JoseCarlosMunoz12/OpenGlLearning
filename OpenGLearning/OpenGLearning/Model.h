#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"


class Model
{
private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 Position;
	void updateUniform()
	{

	}
public:
	Model(glm::vec3 position, Material* material, Texture* orTexDif, Texture* orTexSpec,std::vector<Mesh*> meshes)
	{
		this->Position = position;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;
		for (auto* i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}
		
	}
	~Model()
	{
		for (auto*& i: this->meshes)
		{
			delete i;
		}
	}
	void update()
	{

	}
	void render(Shader* shader)
	{
		//Update Uniforms
		this->updateUniform();
		//Update Uniforms
		this->material->sendToShader(*shader);
		shader->use();
		this->overrideTextureDiffuse->bind(0);
		this->overrideTextureSpecular->bind(1);

		for (auto& i : this->meshes)
		{
			i->render(shader);
		}
	}
};

