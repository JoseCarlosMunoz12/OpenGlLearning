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
	std::vector<GLint> ShadowTex;
	//All Shadow Textures
	std::vector<GLint> PntShadow;
	std::vector<GLint> CnShadow;
	std::vector<GLint> ArShadow;
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
	int GetMatId()
	{
		return this->MatId;
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
		int Value = 0;
		for (auto& ii : LightMatix)
		{
			std::string LightPos = "AllDirInfo[" + std::to_string(Value) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(ii, LightPos.c_str());
			Value++;
		}
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
		int Value = 0;
		for (auto& ii : LightMatix)
		{
			std::string LightPos = "AllDirInfo[" + std::to_string(Value) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(ii, LightPos.c_str());
			Value++;
		}

		for (int ii = 0; ii < this->CnShadow.size(); ii++)
		{
			std::string LghtMatrix = "AllCnInfo[" + std::to_string(ii) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(LightMatix[Value], LghtMatrix.c_str());
			Value++;
		}
		for (int ii = 0; ii < this->ArShadow.size(); ii++)
		{
			std::string LighMatrix = "AllArInfo[" + std::to_string(ii) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(LightMatix[Value], LighMatrix.c_str());
			Value++;
		}
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
			GLint diffuseTex, GLint specularTex,
			std::vector<GLint> InitShadowTx = { 2 },
			std::vector<GLint> InitCnShadow = {},
			std::vector<GLint> InitPntShadow = {},
			std::vector<GLint> InitArShadow = {})
			:StdMat(Name, SetId, ShaderId)
	{
		this->ShadowTex = InitShadowTx;
		this->CnShadow = InitCnShadow;
		this->PntShadow = InitPntShadow;
		this->ArShadow = InitArShadow;
		this->SkyClr = SkyColor;
		this->Ambient = ambient;
		this->Diffuse = diffuse;
		this->Specular = specular;
		this->TexIndex.push_back(diffuseTex);
		this->TexIndex.push_back(specularTex);
	}
	void SendToShader(std::vector<Shader*>& program) override
	{
		program[this->ShaderID]->setVec3f(this->Ambient, "material.ambient");
		program[this->ShaderID]->setVec3f(this->Diffuse, "material.diffuse");
		program[this->ShaderID]->setVec3f(this->Specular, "material.specular");
		program[this->ShaderID]->set1i(this->TexIndex[0], "material.diffuseTex");
		program[this->ShaderID]->set1i(this->TexIndex[1], "material.speculartex");
		int Value = 0;
		for (auto& ii : this->ShadowTex)
		{

			std::string lShadow = "AllDirInfo[" + std::to_string(Value) + "].LightShadow";
			program[this->ShaderID]->set1i(ii,lShadow.c_str());
			Value++;
		}
		Value = 0;
		for (auto& ii : this->CnShadow)
		{
			std::string CShadow = "AllCnInfo[" + std::to_string(Value) + "].LightShadow";
			program[this->ShaderID]->set1i(ii,CShadow.c_str());
			Value++;
		}
		Value = 0;
		for (auto& ii : this->ArShadow)
		{
			std::string AShadow = "AllArInfo[" + std::to_string(Value) + "].LightShadow";
			program[this->ShaderID]->set1i(ii, AShadow.c_str());
			Value++;
		}
		program[this->ShaderID]->setVec3f(this->SkyClr, "SkyColor");
	}
	void SendToShader(std::vector<Shader*>& program, std::vector<glm::mat4> LightMatix) override
	{
		this->SendToShader(program);
		int Value = 0;
		for (int ii = 0; ii < this->ShadowTex.size();ii++)
		{
			std::string LIghtMatrix = "AllDirInfo[" + std::to_string(Value) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(LightMatix[Value], LIghtMatrix.c_str());
			Value++;
		}
		for (int ii = 0; ii < this->CnShadow.size(); ii++)
		{
			std::string LghtMatrix = "AllCnInfo[" + std::to_string(ii) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(LightMatix[Value], LghtMatrix.c_str());
			Value++;
		}
		for (int ii = 0; ii < this->ArShadow.size(); ii++)
		{
			std::string LighMatrix = "AllArInfo[" + std::to_string(ii) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(LightMatix[Value], LighMatrix.c_str());
			Value++;
		}
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
		std::vector<GLint> InitShadowTex,
		std::vector<GLint> InitCnShadow = {},
		std::vector<GLint> InitPntShadow = {},
		std::vector<GLint> InitArShadow = {},
		glm::vec3 ambient = glm::vec3(0.1f),
		glm::vec3 diffuse = glm::vec3(1.0f),
		glm::vec3 specular = glm::vec3(1.0f))
		:StdMat(Name, SetId, ShaderId)
	{
		this->ShadowTex = InitShadowTex;
		this->CnShadow = InitCnShadow;
		this->PntShadow = InitPntShadow;
		this->ArShadow = InitArShadow;
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
		int Value = 0;
		for (auto& ii : this->ShadowTex)
		{
			std::string lShadow = "AllDirInfo[" + std::to_string(Value) + "].LightShadow";
			program[this->ShaderID]->set1i(ii, lShadow.c_str());
			Value++;
		}
		Value = 0;
		for (auto& ii : this->CnShadow)
		{
			std::string CShadow = "AllCnInfo[" + std::to_string(Value) + "].LightShadow";
			program[this->ShaderID]->set1i(ii, CShadow.c_str());
			Value++;
		}
		Value = 0;
		for (auto& ii : this->ArShadow)
		{
			std::string AShadow = "AllArInfo[" + std::to_string(Value) + "].LightShadow";
			program[this->ShaderID]->set1i(ii, AShadow.c_str());
			Value++;
		}
		program[this->ShaderID]->setVec3f(this->SkyClr, "SkyColor");
	}
	void SendToShader(std::vector<Shader*>& program, std::vector<glm::mat4> LightMatix) override
	{
		this->SendToShader(program);
		int Value = 0;
		for (int ii = 0; ii < this->ShadowTex.size(); ii++)
		{
			std::string LghtMatrix = "AllDirInfo[" + std::to_string(Value) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(LightMatix[ii], LghtMatrix.c_str());
			Value++;
		}
		for (int ii = 0; ii < this->CnShadow.size(); ii++)
		{
			std::string LghtMatrix = "AllCnInfo[" + std::to_string(ii) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(LightMatix[Value], LghtMatrix.c_str());
			Value++;
		}
		for (int ii = 0; ii < this->ArShadow.size(); ii++)
		{
			std::string LighMatrix = "AllArInfo[" + std::to_string(ii) + "].LightMatrix";
			program[this->ShaderID]->setMat4fv(LightMatix[Value],LighMatrix.c_str());
			Value++;
		}
	}
};