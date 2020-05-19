#pragma once
#include "Phy_Base.h"
namespace CoatlPhysicsEngine {
	class Phy_Grav: public Phy_Base
	{
		glm::vec3 Grav;
	public:
		Phy_Grav(glm::vec3 InitGrav);
		~Phy_Grav();
		glm::vec3 GetForce(Particle P0) override;
		glm::vec3 GetForce(Particle P0, Particle P1) override;
		glm::vec3 GetForce(Particle P0,float dt) override;
	};
}