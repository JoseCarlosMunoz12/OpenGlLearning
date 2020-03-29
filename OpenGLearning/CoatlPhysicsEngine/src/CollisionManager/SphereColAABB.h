#pragma once
#include "../Shapes/Sphere.h"
#include "../Shapes/AABB_Obj.h"
namespace CoatlPhysicsEngine {
	class SphereColAABB
	{
	public:
		SphereColAABB();
		~SphereColAABB();
		bool  SphereColsAABB(Sphere Sph0,AABB_Obj Obj0);			
	};
}
