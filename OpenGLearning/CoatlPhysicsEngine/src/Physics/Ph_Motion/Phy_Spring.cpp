#include "Phy_Spring.h"
using namespace CoatlPhysicsEngine;

Phy_Spring::Phy_Spring(glm::vec3 SetPos,float InitK, float InitRLength)
{
	this->K = InitK;
	this->Lock_Pos = SetPos;
	this->RestLength = InitRLength;
}

Phy_Spring::~Phy_Spring()
{
}

glm::vec3 Phy_Spring::GetForce(Particle P0)
{	
	glm::vec3 F = P0.GetPos();
	F = F - Lock_Pos;
	float Mag = glm::length(F);
	Mag = glm::abs(Mag - RestLength);
	Mag = K * Mag;
	F = glm::normalize(F);
	F = -Mag * F;
	return F;
}

glm::vec3 Phy_Spring::GetForce(Particle P0, Particle P1)
{
	return glm::vec3();
}
