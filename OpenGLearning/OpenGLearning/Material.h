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
class StdMat
{
private:
	std::string MatName;
	int MatId;
protected:
	std::vector<GLint> TexIndex;
	glm::vec3 SkyClr;
	int ShaderID;
public:
	StdMat(std::string Name,int SetId, int ShaderId)
	{
		this->MatName = Name;
		this->MatId = SetId;
		this->ShaderID = ShaderId;
	}
	~StdMat()
	{
		std::cout << "Test\n";
	}
	virtual void sendToShader(std::vector<Shader*>& program)
	{
	}
	const char* GetName()
	{
		return this->MatName.c_str();
	}
	int GetShaderId()
	{
		return this->ShaderID;
	}

};

class Standardmat : public StdMat
{
public:
	Standardmat(std::string Name, int SetId, int ShaderId)
		:StdMat(Name,SetId,ShaderID)
	{
	}
};

class TxtMat : public StdMat
{
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
public:
	TxtMat(std::string Name, int SetId,int ShaderId,
			glm::vec3 SkyColor, glm::vec3 ambient,
			glm::vec3 diffuse, glm::vec3 specular,
			GLint diffuseTex, GLint specularTex)
			:StdMat(Name, SetId, ShaderId)
	{
		this->SkyClr = SkyColor;
		this->Ambient = ambient;
		this->Diffuse = diffuse;
		this->Specular = specular;
		this->TexIndex.push_back(diffuseTex);
		this->TexIndex.push_back(specularTex);
	}
	void sendToShader(std::vector<Shader*>& program)
	{
		program[this->ShaderID]->setVec3f(this->Ambient, "material.ambient");
		program[this->ShaderID]->setVec3f(this->Diffuse, "material.diffuse");
		program[this->ShaderID]->setVec3f(this->Specular, "material.specular");
		program[this->ShaderID]->set1i(this->TexIndex[0], "material.diffuseTex");
		program[this->ShaderID]->set1i(this->TexIndex[1], "material.speculartex");
		program[this->ShaderID]->setVec3f(this->SkyClr, "SkyColor");
	}
	~TxtMat()
	{
		std::cout << "gone\n";
	}
};

class MipMapMat : public StdMat
{
public:
	MipMapMat(std::string Name, int SetId, int ShaderId,
		glm::vec3 SkyColor,
		std::vector<GLint> TexIndex)
		:StdMat(Name, SetId, ShaderId)
	{
		this->SkyClr = SkyColor;
		this->TexIndex = TexIndex;
	}
	void sendToShader(std::vector<Shader*>& program)
	{
		program[this->ShaderID]->set1i(this->TexIndex[0], "Texture0");
		program[this->ShaderID]->set1i(this->TexIndex[1], "Texture1");
		program[this->ShaderID]->set1i(this->TexIndex[2], "Texture2");
		program[this->ShaderID]->set1i(this->TexIndex[3], "Texture3");
		program[this->ShaderID]->set1i(this->TexIndex[4], "Texture4");
		program[this->ShaderID]->setVec3f(this->SkyClr, "SkyColor");
	}
};
// trying to devide the class into two Tex and MipMap
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