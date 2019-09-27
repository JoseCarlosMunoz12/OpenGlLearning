#pragma once
#include <glm.hpp>

class Lights
{
	glm::vec3 Position;
	glm::vec3 Color;
public:	
	Lights(glm::vec3 Pos,glm::vec3 Clr)
	{
		this->Position = Pos;
		this->Color = Clr;
	}
	glm::mat4 GetLightMatrix(glm::vec3 WorldView)
	{
		float NearPlane = 1.f,FarPlane = 10.f;
		glm::mat4 LightProj = glm::ortho(-10.f, 10.f, -10.f, 10.f,NearPlane,FarPlane );
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
	//Get Info
	glm::vec3 GetPos()
	{
		return this->Position;
	}
	glm::vec3 GetColor()
	{
		return this->Color;
	}
};