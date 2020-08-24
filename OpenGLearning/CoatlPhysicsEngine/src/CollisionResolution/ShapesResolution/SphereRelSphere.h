#pragma once
#include "../Col_Resolution.h"
#include "../../Shapes/Sphere.h"
namespace CoatlPhysicsEngine {
	class SphereRelSphere
	{
	protected:
		std::vector<std::shared_ptr<Contacts>> SphRelSph(Sphere Sph0,Sphere Sph1);
	public:
		SphereRelSphere();
		~SphereRelSphere();
	};
}