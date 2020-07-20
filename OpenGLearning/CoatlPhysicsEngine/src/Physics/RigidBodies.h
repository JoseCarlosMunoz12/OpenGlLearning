#pragma once
#include "Particle.h"
namespace CoatlPhysicsEngine {
	class RigidBodies : public Particle
	{
	private:
	public:
		RigidBodies(glm::vec3 Pos);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp);
		~RigidBodies();
	};	
}
