#pragma once
#include "../Shapes/Capsule.h"
#include "../Shapes/Triangles.h"
namespace CoatlPhysicsEngine {
	class TriangleColCapsule
	{
	protected:
		bool TrColCap(Triangles Tr, Capsule Cap);
	public:
		TriangleColCapsule();
		~TriangleColCapsule();
	};
}
