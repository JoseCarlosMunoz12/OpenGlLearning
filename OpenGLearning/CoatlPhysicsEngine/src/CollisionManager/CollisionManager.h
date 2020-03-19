#pragma once
#include "SphereColSphere.h"
namespace CoatlPhysicsEngine {
	class CollisionManager
	{
	private:
		std::unique_ptr<SphereColSphere> S_Col;
	public:
		CollisionManager();
		~CollisionManager();
	};
}

