#pragma once
#include "../Shapes/OBB.h"
#include "../Shapes/AABB_Obj.h"
namespace CoatlPhysicsEngine {
	class ABBColOBB
	{
	protected:
		bool ABBColsOBB(AABB_Obj ABB, OBB Obj);
	public:
		ABBColOBB();
		~ABBColOBB();
	};
}
