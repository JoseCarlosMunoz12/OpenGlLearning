#pragma once
#include "../Col_Resolution.h"
#include "../../Shapes/AABB_Obj.h"
#include "../../Shapes/Capsule.h"
#include "../../GJK/GJK.h"
namespace CoatlPhysicsEngine
{
	class CapsuleRelAABB
	{
	private:
		
	protected:
		std::vector<std::shared_ptr<Contacts>> CapRelAABB(Capsule Cap, AABB_Obj Obj);
	public:
		CapsuleRelAABB();
		~CapsuleRelAABB();
	};
}