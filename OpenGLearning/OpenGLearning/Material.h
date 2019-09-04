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
	Standardmat(std::string Name, int SetId, int ShaderId, glm::vec3 SkyColor)
		:StdMat(Name,SetId,ShaderID)
	{
		this->SkyClr = SkyColor;
	}
	void SendToShader(std::vector<Shader*>& program)
	{
		program[this->ShaderID]->setVec3f(this->SkyClr, "SkyColor");
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