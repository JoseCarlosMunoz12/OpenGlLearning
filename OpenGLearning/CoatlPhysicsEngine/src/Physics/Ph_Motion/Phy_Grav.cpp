#include "Phy_Grav.h"
using namespace CoatlPhysicsEngine;

Phy_Grav::Phy_Grav(glm::vec3 InitGrav)
	:Phy_Base()
{
	this->Grav = InitGrav;
}

Phy_Grav::~Phy_Grav()
{
}

glm::vec3 Phy_Grav::GetForce(Particle P0)
{
	float Mass = P0.GetMass();
	return Mass * this->Grav;
}

glm::vec3 Phy_Grav::GetForce(Particle P0, Particle P1)
{
	return glm::vec3(0.f);
}
