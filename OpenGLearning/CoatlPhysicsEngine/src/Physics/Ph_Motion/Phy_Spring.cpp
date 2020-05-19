#include "Phy_Spring.h"
using namespace CoatlPhysicsEngine;

Phy_Spring::Phy_Spring(glm::vec3 SetPos,float InitK,float Damping)
{
	this->Damping = Damping;
	this->K = -InitK;
	this->Lock_Pos = SetPos;
}

Phy_Spring::~Phy_Spring()
{
}

glm::vec3 Phy_Spring::GetForce(Particle P0)
{

	return K * (P0.GetPos() - Lock_Pos) - P0.GetVel() * Damping;
}

glm::vec3 Phy_Spring::GetForce(Particle P0, Particle P1)
{
	return glm::vec3(0.f);
}

glm::vec3 CoatlPhysicsEngine::Phy_Spring::GetForce(Particle P0, float dt)
{
	return glm::vec3(0.f);
}
