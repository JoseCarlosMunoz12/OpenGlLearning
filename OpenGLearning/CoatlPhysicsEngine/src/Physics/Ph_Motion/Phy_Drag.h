#pragma once
#include "Phy_Base.h"
namespace CoatlPhysicsEngine {
	class Phy_Drag :public Phy_Base
	{
	private:
		float K0;
		float K1;
	public:
		Phy_Drag(float InitK0, float InitK1);
		Phy_Drag();
		~Phy_Drag();
		glm::vec3 GetForce(Bod_Base P0,Bod_Base P1) override;
		glm::vec3 GetForce(Bod_Base P0) override;
		glm::vec3 GetForce(Bod_Base P0,float dt) override;
	};}
