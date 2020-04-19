#pragma once
#include <algorithm>
#include "../Shapes/Triangles.h"
#include "../Shapes/AABB_Obj.h"
#include "../Shapes/SimpleShapes.h"
namespace CoatlPhysicsEngine {
	class TriangleColAABB
	{
	private:
		bool PlaneABB(Plane Pl, AABB_Obj Obj);
	protected:
		bool TrColAABB(Triangles Tr, AABB_Obj AABB);
	public:
		TriangleColAABB();
		~TriangleColAABB();
	};
}
