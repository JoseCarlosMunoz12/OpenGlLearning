#pragma once

#include "../Shapes/AABB_Obj.h"
#include "../Shapes/Capsule.h"
#include "TriangleColCapsule.h"

namespace CoatlPhysicsEngine {
	class CapsuleColAABB : public TriangleColCapsule
	{
	private:
	protected:
		bool CapColAABB(Capsule Cap,AABB_Obj Obj);
	public:
		CapsuleColAABB();
		~CapsuleColAABB();
	};
}