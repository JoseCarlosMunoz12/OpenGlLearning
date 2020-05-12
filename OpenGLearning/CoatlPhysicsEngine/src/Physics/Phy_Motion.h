#pragma once
#include "Particle.h"
namespace CoatlPhysicsEngine {
	class Phy_Motion
	{
	protected:
		glm::vec3 UpdateParPos(glm::vec3 Pos, glm::vec3 Ac, float dt,
			std::shared_ptr<Particle> Part);
	public:
		Phy_Motion();
		~Phy_Motion();
	};
}