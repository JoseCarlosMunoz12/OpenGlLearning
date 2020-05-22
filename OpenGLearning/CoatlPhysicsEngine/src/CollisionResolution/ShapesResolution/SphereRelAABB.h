#pragma once
#include "../Manifoldsh.h"
#include "../../Shapes/Sphere.h"
#include "../../Shapes/AABB_obj.h"
namespace CoatlPhysicsEngine {
	class SphereRelAABB
	{
	protected:
		std::vector<std::shared_ptr<Contact>> SphRelAABB(Sphere Sph0, AABB_Obj Obj);
	public:
		SphereRelAABB();
		~SphereRelAABB();
	};
}