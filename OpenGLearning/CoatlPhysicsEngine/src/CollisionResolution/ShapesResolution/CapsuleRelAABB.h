#pragma once
#include "../Manifoldsh.h"
#include "../../Shapes/AABB_Obj.h"
#include "../../Shapes/Capsule.h"
#include "../../GJK/GJK.h"
namespace CoatlPhysicsEngine
{
	class CapsuleRelAABB
	{
	private:
		int line_support(glm::vec3& Support, glm::vec3 D, Capsule Cap);
		int poly_support(glm::vec3 & Support, glm::vec3 D, AABB_Obj obj);
	protected:
		std::vector<std::shared_ptr<Contact>> CapRelAABB(Capsule Cap, AABB_Obj Obj);
	public:
		CapsuleRelAABB();
		~CapsuleRelAABB();
	};
}