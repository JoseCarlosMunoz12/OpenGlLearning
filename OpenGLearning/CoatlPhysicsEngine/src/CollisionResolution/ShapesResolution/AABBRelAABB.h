#pragma once
#include "../Col_Resolution.h"
#include "../../Shapes/AABB_Obj.h"
namespace CoatlPhysicsEngine {
	class AABBRelAABB
	{
	protected:
		std::vector<std::shared_ptr<Contacts>> AABB_Rel(AABB_Obj Obj0, AABB_Obj Obj1);
	public:
		AABBRelAABB();
		~AABBRelAABB();
	};
}