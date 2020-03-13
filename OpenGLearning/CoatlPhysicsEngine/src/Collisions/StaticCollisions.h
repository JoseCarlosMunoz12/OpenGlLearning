#pragma once
#include <string>
#include <iostream>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>

#include "CollisionWorld.h"

namespace CoatlPhysicsEngine 
{
	class StaticCollisions : CollisionWorld
	{
	private:
		glm::vec3 Pos;
	public:
		StaticCollisions(std::string Name,glm::vec3 InitPos = glm::vec3(0.f));
		~StaticCollisions();
		void ShowName();
		glm::vec3 GetPos();
		void SetPos(glm::vec3 NewPos);
	};
}

