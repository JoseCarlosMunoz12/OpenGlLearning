#pragma once
#include "../Shapes/Triangles.h"
#include "../Shapes/SimpleShapes.h"
namespace CoatlPhysicsEngine {
	class TriangleColTriangle
	{
	protected:
	public:
		TriangleColTriangle();
		~TriangleColTriangle();
		bool TrColTr(Triangles Tr0, Triangles Tr1);
	};
}
