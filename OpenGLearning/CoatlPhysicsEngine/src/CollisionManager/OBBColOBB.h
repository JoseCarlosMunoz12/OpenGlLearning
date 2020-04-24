#pragma once
#include "../Shapes/OBB.h"
namespace CoatlPhysicsEngine {
	class OBBColOBB
	{
	protected:
	public:
		OBBColOBB();
		~OBBColOBB();
		bool OBBCol(OBB Ob0, OBB Ob1);
	};
}