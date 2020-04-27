#pragma once
#include "../Shapes/Sphere.h"
#include "../Shapes/OBB.h"
namespace CoatlPhysicsEngine {
	class SphereColOBB
	{
	protected:
		bool SphColOBB(Sphere Sph, OBB Obj);
	public:
		SphereColOBB();
		~SphereColOBB();
	};

}