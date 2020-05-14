#pragma once

#include "../Shapes/AABB_Obj.h"
#include "../Shapes/Capsule.h"

namespace CoatlPhysicsEngine {
	class CapsuleColAABB
	{
	private:
	protected:
		bool CapColAABB(Capsule Cap,AABB_Obj Obj);
	public:
		CapsuleColAABB();
		~CapsuleColAABB();
	};
}