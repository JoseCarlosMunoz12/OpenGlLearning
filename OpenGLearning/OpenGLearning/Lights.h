#pragma once
#include <glm.hpp>
struct OrthoView
{
	float Left;
	float Right;
	float Bottom;
	float Up;
	float FarPlane;
};
//Base Class of lights
class MainLight
{
protected:
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec3 Front;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	int LightID;
	float Pitch, Yaw;
public:
	MainLight(glm::vec3 InitColor,glm::vec3 InitPos,float InitPitch, float InitYaw,int InitId)
	{
		this->SetPosition(InitPos);
		this->SetPitch(InitPitch);
		this->SetYaw(InitYaw);
		this->Color = InitColor;
		this->LightID = InitId;
	}
	//Set Values
	void SetPosition(glm::vec3 NewPos)
	{
		this->Position = NewPos;
	}
	void SetColor(glm::vec3 NewColor)
	{
		this->Color = NewColor;
	}
	void SetYaw(float NewYaw)
	{
		this->Yaw = NewYaw;
	}
	void SetPitch(float NewPitch)
	{
		this->Pitch = NewPitch;
	}
	void SetAmbient(glm::vec3 NewAmbient)
	{
		this->Ambient = NewAmbient;
	}
	void SetDiffuse(glm::vec3 NewDiffuse)
	{
		this->Diffuse = NewDiffuse;
	}
	void SetSpecular(glm::vec3 NewSpecular)
	{
		this->Specular = NewSpecular;
	}
	// Get Values
	glm::vec3 GetColor()
	{
		return this->Color;
	}
	float GetYaw()
	{
		return this->Yaw;
	}
	float GetPitch()
	{
		return this->Pitch;
	}
	glm::vec3 GetPos()
	{
		return this->Position;
	}
	glm::vec3 GetAmbient()
	{
		return this->Ambient;
	}
	glm::vec3 GetDiffuse()
	{
		return this->Diffuse;
	}
	glm::vec3 GetSpecular()
	{
		return this->Specular;
	}
	//MoveLight
	void Move(glm::vec3 Move)
	{
		this->Position += Move;
	}
	//Update functions
	void UpdateFront()
	{
		this->Front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front.y = sin(glm::radians(this->Pitch));
		this->Front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));

	}
	virtual glm::mat4 GetLightMatrix(glm::vec3 WorldView) = 0;
	
};
// Classes of Lights
class DrLights : public MainLight
{
	OrthoView Views;
	int Width, Height;
public:
	DrLights(int LightId, glm::vec3 InitPos, glm::vec3 InitCol, 
		int FrameWidth, int FrameHeight,
		glm::vec3 InitAmbient = glm::vec3(.5f),
		glm::vec3 InitDiffuse = glm::vec3(.5f),
		glm::vec3 InitSpecular = glm::vec3(.5f),
		float InitPitch = -75.f, float InitYaw = 90.f )
		:MainLight(InitCol,InitPos,InitPitch,InitYaw,LightId)
	{
		//Light Properties
		this->Ambient = InitAmbient;
		this->Diffuse = InitDiffuse;
		this->Specular = InitSpecular;
		//viewing
		this-> Width = FrameWidth;
		this->Height = FrameHeight;
		//MatrixInfo
		this->Views.Left = -10.f;
		this->Views.Right = 10.f;
		this->Views.Bottom = -10.f;
		this->Views.Up = 10.f;
		this->Views.FarPlane = 10.f;
		//Position
		this->Position = InitPos;

		this->UpdateFront();
	}
	glm::mat4 GetLightMatrix(glm::vec3 WorldView) override
	{
		glm::mat4 LightProj;
		float NearPlane = 1.f;
	
		LightProj = glm::ortho(Views.Left, Views.Right, Views.Bottom, Views.Up, NearPlane, Views.FarPlane);
	
		this->UpdateFront();
		glm::mat4 LightView = glm::lookAt(this->Position, this->Position + this->Front, WorldView);
		return LightProj * LightView;
	}
	//Get Parts
	OrthoView GetOrthoParts()
	{
		return this->Views;
	}
	//Set Parts
	void SetOrthoParts(OrthoView NewParts)
	{
		this->Views = NewParts;
	}
};

class PntLights : public MainLight
{
	float Constant;
	float Linear;
	float Quadratic;
	int Width, Height;
	float Near = 1.0f;
	float Far = 25.f;
public:
	PntLights(glm::vec3 InitCol, glm::vec3 InitPos,int LightId)
		:MainLight(InitCol,InitPos,0.f,0.f, LightID)
	{

	}
	//Set values
	void SetConstant(float NewConst)
	{
		this->Constant = NewConst;
	}
	void SetLinear(float NewLinear)
	{
		this->Linear = NewLinear;
	}
	void SetQuad(float NewQuad)
	{
		this->Quadratic = NewQuad;
	}
	//get Values
	float GetConst()
	{
		return this->Constant;
	}
	float GetLinear()
	{
		return this->Linear;
	}
	float GetQuad()
	{
		return this->Quadratic;
	}
	std::vector<glm::mat4> GetMatrixes()
	{
		float aspect = (float)this->Width / (float)this->Height;
		glm::mat4 ShadowProj = glm::perspective(glm::radians(90.f), aspect,this->Near,this->Far);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(ShadowProj *
			glm::lookAt(this->Position, this->Position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(ShadowProj *
			glm::lookAt(this->Position, this->Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(ShadowProj *
			glm::lookAt(this->Position, this->Position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(ShadowProj *
			glm::lookAt(this->Position, this->Position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(ShadowProj *
			glm::lookAt(this->Position, this->Position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(ShadowProj *
			glm::lookAt(this->Position, this->Position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		return shadowTransforms;
	}
};