#pragma once
#include <glm.hpp>

class Lights
{
	glm::vec3 Position;
	glm::vec3 Color;
	float Fov;
	int Width,Height;
	bool OrthoView;
public:	
	Lights(glm::vec3 Pos,glm::vec3 Clr,int FrameWidth,int FrameHeight,bool InitOrtho = true)
		:Fov(45.f)
	{
		this->Position = Pos;
		this->Color = Clr;
		this->Width = FrameWidth;
		this->Height = FrameHeight;
		this->OrthoView = InitOrtho;
		if (this->OrthoView)
		{

		}
	}
	glm::mat4 GetLightMatrix(glm::vec3 WorldView)
	{
		glm::mat4 LightProj;
		float NearPlane = 1.f,FarPlane = 10.f;
		if (OrthoView){	
			LightProj = glm::ortho(-10.f, 10.f, -10.f, 10.f,NearPlane,FarPlane );
		}
		else{
			NearPlane = 1.f;
			FarPlane = 75.f;
			LightProj = glm::perspective(glm::radians(this->Fov),
				static_cast<float>(this->Width)/static_cast<float>(this->Height),
				NearPlane,
				FarPlane);
		}
		glm::mat4 LightView = glm::lookAt(this->Position, this->Position + glm::vec3(0.f,-1.f,1.f), WorldView);
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
		this->OrthoView = OrthoChoesn;
	}
	//Get Info
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
		return this->OrthoView;
	}
};