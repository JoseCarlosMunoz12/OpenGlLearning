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
	int ViewType;
	float Pitch, Yaw;
public:
	MainLight(glm::vec3 InitColor,glm::vec3 InitPos,float InitPitch, float InitYaw)
	{
		this->SetPosition(InitPos);
		this->SetPitch(InitPitch);
		this->SetYaw(InitYaw);
		this->Color = InitColor;
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
	DrLights( glm::vec3 InitPos, glm::vec3 InitCol, 
		int FrameWidth, int FrameHeight,
		float InitPitch = -75.f, float InitYaw = 90.f )
		:MainLight(InitCol,InitPos,InitPitch,InitYaw)
	{
		this-> Width = FrameWidth;
		this->Height = FrameHeight;

		this->Views.Left = -10.f;
		this->Views.Right = 10.f;
		this->Views.Bottom = -10.f;
		this->Views.Up = 10.f;
		this->Views.FarPlane = 10.f;

		this->Position = InitPos;
		this->UpdateFront();
	}
	glm::mat4 GetLightMatrix(glm::vec3 WorldView) 
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
public:
	PntLights(glm::vec3 InitCol, glm::vec3 InitPos)
		:MainLight(InitCol,InitPos,0.f,0.f)
	{

	}
};

class Lights
{	
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec3 Front;
	float Fov;
	int Width,Height;
	float Pitch, Yaw;
	OrthoView Views;
	bool Ortho;
	void UpdateFront()
	{
		this->Front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front.y = sin(glm::radians(this->Pitch));
		this->Front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	}
public:	
	Lights(glm::vec3 Pos,glm::vec3 Clr,
		int FrameWidth,int FrameHeight,bool InitOrtho = true)
		:Fov(45.f)
	{
		this->Views.Left = -10.f;
		this->Views.Right = 10.f;
		this->Views.Bottom = -10.f;
		this->Views.Up = 10.f;
		this->Views.FarPlane = 10.f;

		this->Position = Pos;
		this->Color = Clr;
		this->Width = FrameWidth;
		this->Height = FrameHeight;
		this->Ortho = InitOrtho;
		this->Pitch = -75.f;
		this->Yaw = 90.f;
		this->UpdateFront();
	}
	glm::mat4 GetLightMatrix(glm::vec3 WorldView)
	{
		glm::mat4 LightProj;
		float NearPlane = 1.f;
		if (Ortho){	
			LightProj = glm::ortho(Views.Left, Views.Right, Views.Bottom, Views.Up, NearPlane,Views.FarPlane );
		}
		else{
			LightProj = glm::perspective(glm::radians(this->Fov),
				static_cast<float>(this->Width)/static_cast<float>(this->Height),
				NearPlane,
				Views.FarPlane);
		}
		this->UpdateFront();
		glm::mat4 LightView = glm::lookAt(this->Position, this->Position + this->Front, WorldView);
		return LightProj * LightView;
	}
	//MoveLight
	void Move(glm::vec3 Move)
	{
		this->Position += Move;
	}
	//Set Info
	void SetPosition(glm::vec3 NewPos)
	{
		this->Position = NewPos;
	}
	void SetColor(glm::vec3 NewColor)
	{
		this->Color = NewColor;
	} 
	void SetView(bool OrthoChoesn)
	{
		this->Ortho = OrthoChoesn;
	}
	void SetYaw(float NewYaw)
	{
		this->Yaw = NewYaw;
	}
	void SetPitch(float NewPitch)
	{
		this->Pitch = NewPitch;
	}
	void SetOrthoView(OrthoView NewOrtho)
	{
		this->Views = NewOrtho;
	}
	//Get Info
	OrthoView GetOrtho()
	{
		return this->Views;
	}
	glm::vec3 GetPos()
	{
		return this->Position;
	}
	glm::vec3 GetColor()
	{
		return this->Color;
	}
	bool GetChossenView()
	{
		return this->Ortho;
	}
	float GetYaw()
	{
		return this->Yaw;
	}
	float GetPitch()
	{
		return this->Pitch;
	}
};