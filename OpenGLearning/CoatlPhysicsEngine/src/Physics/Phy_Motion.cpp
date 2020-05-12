#include "Phy_Motion.h"
using namespace CoatlPhysicsEngine;

glm::vec3 Phy_Motion::UpdateParPos(glm::vec3 Pos, glm::vec3 Ac, float dt, std::shared_ptr<Particle> Part)
{
	return Part->UpdatePos(Pos, Ac, dt);
}

Phy_Motion::Phy_Motion()
{
}

Phy_Motion::~Phy_Motion()
{
}
