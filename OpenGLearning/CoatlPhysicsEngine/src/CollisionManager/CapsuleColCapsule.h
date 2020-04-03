#pragma once
#include "../Shapes/Capsule.h"
namespace CoatlPhysicsEngine {
	class CapsuleColCapsule
	{
	protected:
		bool CapsuleCols(Capsule Cap0, Capsule Cap1);
	public:
		CapsuleColCapsule();
	};
}
