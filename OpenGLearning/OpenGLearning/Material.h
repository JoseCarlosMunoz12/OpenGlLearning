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
	glm::vec3 SkyColor;
	std::vector<GLint> TexIndex;
	std::string Name = "Test";
	int MatId;
public:
	Material(glm::vec3 SkyColor,glm::vec3 ambient,	glm::vec3 diffuse, glm::vec3 specular,
			GLint diffuseTex, GLint specularTex,std::string MatName,int SetId)
	{
		this->Name = MatName;
		this-> SkyColor = SkyColor;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->TexIndex.push_back(diffuseTex);
		this->TexIndex.push_back(specularTex);
		this->MatId = SetId;
	}
	Material(glm::vec3 SkyColor,std::vector<GLint> TexIndex,std::string MatName,int SetId)
	{
		this->Name = MatName;
		this->TexIndex = TexIndex;
		this->SkyColor = SkyColor;
		this->MatId = SetId;
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
		program.setVec3f(this->SkyColor, "SkyColor");
	}
	void sendManyTexToShader(Shader& program)
	{
		program.set1i(this->TexIndex[0], "Texture0");
		program.set1i(this->TexIndex[1], "Texture1");
		program.set1i(this->TexIndex[2], "Texture2");
		program.set1i(this->TexIndex[3], "Texture3");
		program.set1i(this->TexIndex[4], "Texture4");
		program.set1i(this->TexIndex[5], "Texture5");
		program.setVec3f(this->SkyColor, "SkyColor");

	}
	const char* GetName()
	{
		return this->Name.c_str();
	}
	int GetMatId()
	{
		return this->MatId;
	}

};