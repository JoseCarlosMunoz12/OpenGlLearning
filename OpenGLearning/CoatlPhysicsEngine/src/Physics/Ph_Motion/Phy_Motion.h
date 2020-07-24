#pragma once
#include "Phy_Base.h"
namespace CoatlPhysicsEngine {
	class Phy_Motion : public Phy_Base
	{
	protected:
		glm::vec3 UpdateParPos(glm::vec3 Pos, float dt,
			std::shared_ptr<Bod_Base> Part);
	public:
		Phy_Motion();
		~Phy_Motion();

	};
}