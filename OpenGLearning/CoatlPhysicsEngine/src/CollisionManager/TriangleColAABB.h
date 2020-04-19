#pragma once
#include <algorithm>
#include "../Shapes/Triangles.h"
#include "../Shapes/AABB_Obj.h"
#include "../Shapes/SimpleShapes.h"
namespace CoatlPhysicsEngine {
	class TriangleColAABB
	{
	protected:
		bool TrColAABB(Triangles Tr, AABB_Obj AABB);
	public:
		TriangleColAABB();
		~TriangleColAABB();
	};
}
