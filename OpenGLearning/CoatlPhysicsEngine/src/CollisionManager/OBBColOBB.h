#pragma once
#include "../Shapes/OBB.h"
namespace CoatlPhysicsEngine {
	class OBBColOBB
	{
	protected:
		bool OBBCol(OBB Ob0, OBB Ob1);
	public:
		OBBColOBB();
		~OBBColOBB();
	};
}