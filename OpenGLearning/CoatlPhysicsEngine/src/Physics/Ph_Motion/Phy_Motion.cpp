#include "Phy_Motion.h"
using namespace CoatlPhysicsEngine;

glm::vec3 Phy_Motion::UpdateParPos(glm::vec3 Pos,float dt, std::shared_ptr<Bod_Base> Part)
{
	return Part->UpdatePos(dt);
}

Phy_Motion::Phy_Motion()
{
}

Phy_Motion::~Phy_Motion()
{
}
