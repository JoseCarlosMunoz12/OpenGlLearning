#pragma once
#include "../Manifoldsh.h"
#include "../../Shapes/AABB_Obj.h"
#include "../../Shapes/Capsule.h"
namespace CoatlPhysicsEngine
{
	class CapsuleRelAABB
	{
	protected:
		std::vector<std::shared_ptr<Contact>> CapRelAABB(Capsule Cap, AABB_Obj Obj);
	public:
		CapsuleRelAABB();
		~CapsuleRelAABB();
	};
}