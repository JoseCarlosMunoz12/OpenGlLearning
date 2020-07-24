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

glm::vec3 Phy_Grav::GetForce(Bod_Base P0)
{
	float Mass = P0.GetMass();
	return Mass * this->Grav;
}

glm::vec3 Phy_Grav::GetForce(Bod_Base P0, Bod_Base P1)
{
	return glm::vec3(0.f);
}

glm::vec3 CoatlPhysicsEngine::Phy_Grav::GetForce(Bod_Base P0, float dt)
{
	return glm::vec3();
}
