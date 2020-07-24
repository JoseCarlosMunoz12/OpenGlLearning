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

glm::vec3 Phy_Drag::GetForce(Bod_Base P0, Bod_Base P1)
{
	return glm::vec3(0.f);
}

glm::vec3 Phy_Drag::GetForce(Bod_Base P0)
{


	glm::vec3 F = P0.GetVel();
	if (glm::dot(F, F) == 0)
		return glm::vec3(0.f);
	float Mag = glm::sqrt(F.x * F.x + F.y * F.y + F.z * F.z);
	float DragCoefficient = -1 * (K0 * Mag + K1 * Mag * Mag);
	F = glm::normalize(F);
	F = DragCoefficient * F;
	return F;
}

glm::vec3 CoatlPhysicsEngine::Phy_Drag::GetForce(Bod_Base P0, float dt)
{
	return glm::vec3();
}
