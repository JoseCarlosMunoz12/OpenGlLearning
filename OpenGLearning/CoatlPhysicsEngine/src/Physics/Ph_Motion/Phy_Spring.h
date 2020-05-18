#pragma once
#include "Phy_Base.h"
namespace CoatlPhysicsEngine {
	class Phy_Spring :public Phy_Base
	{
	private:
		float K;
		glm::vec3 Lock_Pos;
	public:
		Phy_Spring(glm::vec3 SetPos,float InitK);
		~Phy_Spring();
		glm::vec3 GetForce(Particle P0) override;
		glm::vec3 GetForce(Particle P0, Particle P1) override;
	};
}