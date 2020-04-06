#pragma once

#include "../Shapes/AABB_Obj.h"
#include "../Shapes/Capsule.h"

namespace CoatlPhysicsEngine {
	class CapsuleColAABB
	{
	private:
		float ClosestPoint(std::vector<glm::vec3> Cap, std::vector<glm::vec3> Segment);
	protected:
		bool CapColAABB(Capsule Cap,AABB_Obj Obj);
	public:
		CapsuleColAABB();
		~CapsuleColAABB();
	};
}