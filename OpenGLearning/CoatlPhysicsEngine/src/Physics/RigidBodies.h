#pragma once
#include "Particle.h"
namespace CoatlPhysicsEngine {
	class RigidBodies : public Particle
	{
	private:
		glm::mat4 Intertia;
		glm::vec3 RotVel;
		glm::vec3 TorqueAcum;
		float RotDamp;
	public:
		RigidBodies(glm::vec3 Pos);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp);
		void SetInertia(glm::mat4 InitInitertia);
		~RigidBodies();
	};	
}
