#pragma once
#include "Phy_Base.h"
namespace CoatlPhysicsEngine {
	class Phy_Bungee : public Phy_Base
	{
	private:
		float K;
		float Damping;
		glm::vec3 Lock_Pos;
		float RestLength;
	public:
		Phy_Bungee(glm::vec3 Anchor, float InitK, float InitDamp, float R_Length);
		~Phy_Bungee();
		glm::vec3 GetForce(Bod_Base P0) override;
		glm::vec3 GetForce(Bod_Base P0, Bod_Base P1) override;
		glm::vec3 GetForce(Bod_Base P0, float dt) override;
	};
}
