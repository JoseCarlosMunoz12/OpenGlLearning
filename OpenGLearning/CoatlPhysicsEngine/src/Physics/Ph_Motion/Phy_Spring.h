#pragma once
#include "Phy_Base.h"
namespace CoatlPhysicsEngine {
	class Phy_Spring :public Phy_Base
	{
	private:
		float K;
		float Damping;
		glm::vec3 Lock_Pos;
		float RestLength;
	public:
		Phy_Spring(glm::vec3 SetPos, float InitK, float Damping,
			float InitRLength);
		~Phy_Spring();
		glm::vec3 GetForce(Particle P0) override;
		glm::vec3 GetForce(Particle P0, Particle P1) override;
		glm::vec3 GetForce(Particle P0,float dt) override;
	};
}