#pragma once
#include "../Col_Resolution.h"
#include "../../Shapes/Sphere.h"
#include "../../Shapes/AABB_obj.h"
namespace CoatlPhysicsEngine {
	class SphereRelAABB
	{
	protected:
		std::vector<std::shared_ptr<Contacts>> SphRelAABB(Sphere Sph0, AABB_Obj Obj);
	public:
		SphereRelAABB();
		~SphereRelAABB();
	};
}