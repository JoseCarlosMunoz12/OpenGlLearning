#include "Phy_Bungee.h"
using namespace CoatlPhysicsEngine;

Phy_Bungee::Phy_Bungee(glm::vec3 Anchor, float InitK, float InitDamp, float R_Length)
{
	this->Lock_Pos = Anchor;
	this->K = -InitK;
	this->Damping = InitDamp;
	this->RestLength = R_Length;
}

Phy_Bungee::~Phy_Bungee()
{

}

glm::vec3 Phy_Bungee::GetForce(Bod_Base P0)
{
	glm::vec3 F = (P0.GetPos() - Lock_Pos);
	float Mag = glm::length(F);
	if ( Mag <= RestLength)		
		return glm::vec3(0.f);
	Mag = K * (RestLength - Mag);
	F = glm::normalize(F);
	return -Mag * F - P0.GetVel() * Damping;
}

glm::vec3 Phy_Bungee::GetForce(Bod_Base P0, Bod_Base P1)
{
	return glm::vec3();
}

glm::vec3 Phy_Bungee::GetForce(Bod_Base P0, float dt)
{
	return glm::vec3();
}
