#include "Phy_Drag.h"
using namespace CoatlPhysicsEngine;
Phy_Drag::Phy_Drag(float InitK0, float InitK1)
	:Phy_Base()
{
	this->K0 = InitK0;
	this->K1 = InitK1;
}

Phy_Drag::Phy_Drag()
	:Phy_Base(),K0(1),K1(0)
{
}

Phy_Drag::~Phy_Drag()
{
}

glm::vec3 Phy_Drag::GetForce(Particle P0, Particle P1)
{
	glm::vec3 F = P0.GetVel();
	float Mag = glm::length(F);
	float DragCoefficient = -1 * (K0 * Mag + K1 * Mag * Mag);
	F = glm::normalize(F);
	F = DragCoefficient * F;
	return F ;
}
