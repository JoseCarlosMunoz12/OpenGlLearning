#pragma once
#include "../Shapes/Sphere.h"
namespace CoatlPhysicsEngine {
	class SphereColSphere
	{
	public:
		SphereColSphere();
		~SphereColSphere();
		bool Collided(glm::vec3 Pos0,Sphere Sphere0 ,
			glm::vec3 Pos1, Sphere Sphere1);
	};
}
