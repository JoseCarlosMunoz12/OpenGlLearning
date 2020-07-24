#pragma once
#include "Phy_Base.h"
namespace CoatlPhysicsEngine {
	class Phy_Spring :public Phy_Base
	{
	private:
		float K;
		float Damping;
		glm::vec3 Lock_Pos;
	public:
		Phy_Spring(glm::vec3 SetPos, float InitK, float Damping);
		Phy_Spring(float InitK, float Damping);
		~Phy_Spring();
		glm::vec3 GetForce(Bod_Base P0) override;
		glm::vec3 GetForce(Bod_Base P0, Bod_Base P1) override;
		glm::vec3 GetForce(Bod_Base P0,float dt) override;
	};
}