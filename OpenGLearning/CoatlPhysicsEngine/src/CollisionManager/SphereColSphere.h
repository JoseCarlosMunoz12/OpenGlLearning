#pragma once
#include "../Shapes/Sphere.h"
namespace CoatlPhysicsEngine {
	class SphereColSphere
	{
	protected:
		bool SphereSphere(glm::vec3 Pos0,Sphere Sphere0 ,
			glm::vec3 Pos1, Sphere Sphere1);
	public:
		SphereColSphere();
		~SphereColSphere();
	};
}
