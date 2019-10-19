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
	virtual void SendToShader(std::vector<Shader*>& program)
	{
	}
	virtual void SendToShader(std::vector<Shader*>& program,std::vector<glm::mat4> LightMatrix)
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

class SingleTextMat : public StdMat
{
public:
	SingleTextMat(std::string name, int SetId, int IniShaderId, glm::vec3 Skycolor, GLint TexId)
		:StdMat(name, SetId, IniShaderId)
	{
		this->SkyClr = Skycolor;
		this->TexIndex.push_back(TexId);
	}
	void SendToShader(std::vector<Shader*>& program)  override
	{
		program[this->ShaderID]->setVec3f(this->SkyClr, "SkyColor");
		program[this->ShaderID]->set1i(this->TexIndex[0], "Texture0");
	}
	void SendToShader(std::vector<Shader*>& program,std::vector<glm::mat4> LightMatix) override
	{
		this->SendToShader(program);
		program[this->ShaderID]->setMat4fv(LightMatix[0], "LightMatrix");
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
	void SendToShader(std::vector<Shader*>& program) override
	{
		program[this->ShaderID]->setVec3f(this->SkyClr, "SkyColor");
	}
	void SendToShader(std::vector<Shader*>& program,std::vector<glm::mat4> LightMatix) override
	{
		this->SendToShader(program);
		program[this->ShaderID]->setMat4fv(LightMatix[0], "LightMatrix");
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
			GLint diffuseTex, GLint specularTex,GLint ShadowIndex = 2)
			:StdMat(Name, SetId, ShaderId)
	{
		this->SkyClr = SkyColor;
		this->Ambient = ambient;
		this->Diffuse = diffuse;
		this->Specular = specular;
		this->TexIndex.push_back(diffuseTex);
		this->TexIndex.push_back(specularTex);
		this->TexIndex.push_back(ShadowIndex);
	}
	void SendToShader(std::vector<Shader*>& program) override
	{
		program[this->ShaderID]->setVec3f(this->Ambient, "material.ambient");
		program[this->ShaderID]->setVec3f(this->Diffuse, "material.diffuse");
		program[this->ShaderID]->setVec3f(this->Specular, "material.specular");
		program[this->ShaderID]->set1i(this->TexIndex[0], "material.diffuseTex");
		program[this->ShaderID]->set1i(this->TexIndex[1], "material.speculartex");
		program[this->ShaderID]->set1i(this->TexIndex[2], "ShadowTex");
		program[this->ShaderID]->setVec3f(this->SkyClr, "SkyColor");
	}
	void SendToShader(std::vector<Shader*>& program, std::vector<glm::mat4> LightMatix) override
	{
		this->SendToShader(program);
		program[this->ShaderID]->setMat4fv(LightMatix[0], "LightMatrix");
	}
	~TxtMat()
	{
	}
};

class MipMapMat : public StdMat
{
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
public:
	MipMapMat(std::string Name, int SetId, int ShaderId,
		glm::vec3 SkyColor,	std::vector<GLint> TexIndex,
		glm::vec3 ambient = glm::vec3(0.1f),
		glm::vec3 diffuse = glm::vec3(1.0f),
		glm::vec3 specular = glm::vec3(1.0f))
		:StdMat(Name, SetId, ShaderId)
	{
		this->SkyClr = SkyColor;
		this->TexIndex = TexIndex;
		this->Ambient = ambient;
		this->Diffuse = diffuse;
		this->Specular = specular;
	}
	void SendToShader(std::vector<Shader*>& program) override
	{
		program[this->ShaderID]->setVec3f(this->Ambient, "material.ambient");
		program[this->ShaderID]->setVec3f(this->Diffuse, "material.diffuse");
		program[this->ShaderID]->setVec3f(this->Specular, "material.specular");
		program[this->ShaderID]->set1i(this->TexIndex[0], "Texture0");
		program[this->ShaderID]->set1i(this->TexIndex[1], "Texture1");
		program[this->ShaderID]->set1i(this->TexIndex[2], "Texture2");
		program[this->ShaderID]->set1i(this->TexIndex[3], "Texture3");
		program[this->ShaderID]->set1i(this->TexIndex[4], "Texture4");
		program[this->ShaderID]->set1i(this->TexIndex[5], "ShadowTex");
		program[this->ShaderID]->setVec3f(this->SkyClr, "SkyColor");
	}
	void SendToShader(std::vector<Shader*>& program, std::vector<glm::mat4> LightMatix) override
	{
		this->SendToShader(program);
		program[this->ShaderID]->setMat4fv(LightMatix[0], "LightMatrix");
	}
};