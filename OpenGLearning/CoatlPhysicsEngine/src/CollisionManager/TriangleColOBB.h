#pragma once
#include "../Shapes/OBB.h"
#include "../Shapes/Triangles.h"
#include "../Shapes/SimpleShapes.h"
namespace CoatlPhysicsEngine {
	class TriangleColOBB
	{
	protected:
	public:
		TriangleColOBB();
		~TriangleColOBB();
		bool TriColOBB(Triangles Tr, OBB Obj);
	};
}