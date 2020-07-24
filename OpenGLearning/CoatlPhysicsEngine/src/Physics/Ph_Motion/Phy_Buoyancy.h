#pragma once
#include "Phy_base.h"
namespace CoatlPhysicsEngine {
	class Phy_Buoyancy : public Phy_Base
	{
	private:
		float W_Height;
		float W_Max_Depth;
		float W_Floor;
		float W_Density;
	public:
		Phy_Buoyancy(float Floor, float MaxDepth,
			float Height, float Density = 1000.f);
		~Phy_Buoyancy();
		glm::vec3 GetForce(Bod_Base P0) override;
		glm::vec3 GetForce(Bod_Base P0, Bod_Base P1) override;
		glm::vec3 GetForce(Bod_Base P0, float dt) override;
	};
}
