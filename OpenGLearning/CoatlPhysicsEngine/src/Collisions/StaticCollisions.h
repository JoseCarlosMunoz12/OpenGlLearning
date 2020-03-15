#pragma once
#include <string>
#include <iostream>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
#include <vector>

#include "../Shapes/ColShapes.h"
#include "../Bodies/Bodies.h"

namespace CoatlPhysicsEngine 
{
	class StaticCollisions
	{
	private:
		std::vector<Bodies*> AllStatics;
		int NewCurId = 0;
	public:
		StaticCollisions(std::string Name,glm::vec3 InitPos = glm::vec3(0.f));
		~StaticCollisions();
		void UpdateCollisionCheck();
	};
}

