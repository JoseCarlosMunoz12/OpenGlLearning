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
	return glm::vec3();
}

glm::vec3 Phy_Spring::GetForce(Particle P0, Particle P1)
{
	return glm::vec3();
}
