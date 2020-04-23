#pragma once
#include "../Shapes/Triangles.h"
#include "../Shapes/SimpleShapes.h"
namespace CoatlPhysicsEngine {
	class TriangleColTriangle
	{
	protected:
		bool TrColTr(Triangles Tr0, Triangles Tr1);
	public:
		TriangleColTriangle();
		~TriangleColTriangle();
	};
}
