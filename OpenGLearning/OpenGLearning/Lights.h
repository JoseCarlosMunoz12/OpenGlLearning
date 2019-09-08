#pragma once
#include <glm.hpp>

class Lights
{
public:
	Lights(glm::vec3 Pos,glm::vec3 Clr)
	{
		this->position = Pos;
		this->color = Clr;
	}
	glm::vec3 position;
	glm::vec3 color;
	glm::mat4 GetLightMatrix(glm::vec3 WorldView)
	{
		float NearPlane = 1.f,FarPlane = 7.5f;
		glm::mat4 LightProj = glm::ortho(-10.f, 10.f, -10.f, 10.f,NearPlane,FarPlane );
		glm::mat4 LightView = glm::lookAt(this->position, this->position + glm::vec3(1.f, 0.0f, 0.f), WorldView);
		return LightProj * LightView;
	}
};