#pragma once
#include <string>
#include <vector>
#include "DynamicCollisions.h"
#include "StaticCollisions.h"
namespace CoatlPhysicsEngine {
	struct CollisionWorldSetup
	{
		std::string Name;
		glm::vec3 Gravity = glm::vec3(0.f,0.f,-9.81);
	};
	class CollisionWorld
	{
	private:
		glm::vec3 Gravity;
		StaticCollisions* Statics;
		DynamicCollisions* Dynamics;
		std::string WorldName;
	public:
		CollisionWorld(std::string SetWName);
		CollisionWorld(CollisionWorldSetup SetUp);
		~CollisionWorld();
		std::string ShowName();
		glm::vec3 GetGravity();
		void ChangeName(std::string NewName);
		//Create Static Collisions
		StaticCollisions* CreateStaticCol(std::string Name);
	};
}
