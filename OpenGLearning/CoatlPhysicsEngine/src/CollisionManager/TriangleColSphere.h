#pragma once
#include "../Shapes/Sphere.h"
#include "../Shapes/Triangles.h"
namespace CoatlPhysicsEngine {
	class TriangleColSphere
	{
	protected:
		bool TrColSphere(Triangles Tr, Sphere Sph);
	public:
		TriangleColSphere();
		~TriangleColSphere();
	};
}
