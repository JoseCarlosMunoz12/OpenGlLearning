#pragma once

#include "Vertex.h"

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <vector>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
class Collision
{
private:
	float XHighest, XLowest,
		YHighest, YLowest,
		ZHighest, ZLowest;
public:
	Collision()
	{

	}
	~Collision()
	{

	}
	void CreateCollisionBox(std::vector<Vertex> VrtxToEncapsulate)
	{
		this->XHighest = VrtxToEncapsulate[0].position.x;
		this->XLowest = VrtxToEncapsulate[0].position.x;

		this->YHighest = VrtxToEncapsulate[0].position.y;
		this->YLowest = VrtxToEncapsulate[0].position.y;

		this->ZHighest = VrtxToEncapsulate[0].position.z;
		this->ZLowest = VrtxToEncapsulate[0].position.z;
		for (auto& ii : VrtxToEncapsulate)
		{
			//X positions
			if (this->XHighest < ii.position.x)
			{
				this->XHighest = ii.position.x;
			}
			if (this->XLowest > ii.position.x)
			{
				this->XLowest = ii.position.x;
			}
			//Y positions
			if (this->YHighest < ii.position.y)
			{
				this->YHighest = ii.position.y;
			}
			if (this->YLowest > ii.position.y)
			{
				this->YLowest = ii.position.y;
			}
			//Z positions
			if (this->ZHighest < ii.position.z)
			{
				this->ZHighest = ii.position.z;
			}
			if (this->ZLowest > ii.position.z)
			{
				this->ZLowest = ii.position.z;
			}
		}
	}
	void UpdateBoxPos( glm::vec3 position)
	{
		//X position
		this->XHighest += position.x;
		this->XLowest += position.x;
		//Y positin
		this->YHighest += position.y;
		this->YLowest += position.y;
		//Z position
		this->ZHighest += position.z;
		this->ZLowest += position.z;
	}
	void ShowPos()
	{
	}
	bool CheckForCollision(glm::vec3 RayPos)
	{

		return false;
	}
};