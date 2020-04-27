#pragma once
#include "../Shapes/Capsule.h"
#include "../Shapes/OBB.h"
namespace CoatlPhysicsEngine {
	class CapsuleColOBB
	{
	protected:
		bool CapColOBB(Capsule Cap, OBB Obj);
	public:
		CapsuleColOBB();
		~CapsuleColOBB();
	};
}