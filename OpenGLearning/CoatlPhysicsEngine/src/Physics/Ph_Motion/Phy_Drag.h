#pragma once
#include "Phy_Base.h"
namespace CoatlPhysicsEngine {
	class Phy_Drag
	{
	private:
		float K1;
		float K2;
	public:
		Phy_Drag();
		~Phy_Drag();
		glm::vec3 GetForce();
	};}
