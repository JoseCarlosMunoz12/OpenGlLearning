#include "Phy_Motion.h"
using namespace CoatlPhysicsEngine;

glm::vec3 Phy_Motion::UpdateParPos(glm::vec3 Pos,float dt, std::shared_ptr<Particle> Part)
{
	return Part->UpdatePos(Pos, dt);
}

Phy_Motion::Phy_Motion()
{
}

Phy_Motion::~Phy_Motion()
{
}
