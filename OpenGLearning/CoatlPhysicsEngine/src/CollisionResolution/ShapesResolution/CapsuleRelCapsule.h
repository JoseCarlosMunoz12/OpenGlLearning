#pragma once
#include "../Col_Resolution.h"
#include "../../Shapes/Capsule.h"
#include "../../GJK/GJK.h"
namespace CoatlPhysicsEngine {
	class CapsuleRelCapsule
	{
	private:

	public:
		std::vector<std::shared_ptr<Contacts>> CapRel(Capsule Cap0, Capsule Cap1);
		CapsuleRelCapsule();
		~CapsuleRelCapsule();
	};	
}
