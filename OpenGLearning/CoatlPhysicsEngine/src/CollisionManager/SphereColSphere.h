#pragma once
#include "../Shapes/Sphere.h"
namespace CoatlPhysicsEngine {
	class SphereColSphere
	{
	protected:
		bool SphereSphere(Sphere Sphere0,Sphere Sphere1);
	public:
		SphereColSphere();
		~SphereColSphere();
	};
}
