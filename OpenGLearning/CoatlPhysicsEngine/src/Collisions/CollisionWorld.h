#pragma once
#include <string>
#include <vector>
#include "StaticCollisions.h"
#include "DynamicCollisions.h"
namespace CoatlPhysicsEngine {
	struct CollisionWorldSetup
	{
		std::string Name;
		glm::vec3 Gravity;
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
	};
}
