#pragma once
#include <string>
namespace CoatlPhysicsEngine {
	class CollisionWorld
	{
		std::string WorldName;
	public:
		CollisionWorld(std::string SetWName);
		~CollisionWorld();
	};
}
