#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>

#include "Shader.h"

class Material
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	std::vector<GLint> TexIndex;
	GLint diffuseTex;
	GLint specularTex;
public:
	Material(glm::vec3 ambient,	glm::vec3 diffuse, glm::vec3 specular,
		GLint diffuseTex, GLint specularTex)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->TexIndex.push_back(diffuseTex);
		this->TexIndex.push_back(specularTex);
	}
	~Material()
	{

	}
	//Function
	void sendToShader(Shader& program)
	{
		program.setVec3f(this->ambient,"material.ambient");
		program.setVec3f(this->diffuse,"material.diffuse");
		program.setVec3f(this->specular, "material.specular");
		program.set1i(this->TexIndex[0], "material.diffuseTex");
		program.set1i(this->TexIndex[1], "material.speculartex");
	}
	void sendManyTexToShader(Shader& program)
	{
		int Count = 0;
		for (auto& ii : TexIndex)
		{
			program.set1i(ii,"Texture" + Count);
		}
		
	}
};