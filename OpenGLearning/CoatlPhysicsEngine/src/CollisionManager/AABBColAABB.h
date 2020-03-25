#pragma once
#include "../Shapes/AABB_Obj.h"
namespace CoatlPhysicsEngine {

	class AABBColAABB
	{
	public:
		AABBColAABB();
		~AABBColAABB();
		bool AABBAABB(AABB_Obj Obj0, AABB_Obj Obj1 );
	};

}