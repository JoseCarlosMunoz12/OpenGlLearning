#include "Phy_Spring.h"
using namespace CoatlPhysicsEngine;

Phy_Spring::Phy_Spring(glm::vec3 SetPos,float InitK,float Damping)
{
	this->Damping = Damping;
	this->K = -InitK;
	this->Lock_Pos = SetPos;
}

Phy_Spring::Phy_Spring(float InitK, float Damping)
	:Lock_Pos(glm::vec3(0.f))
{
	this->Damping = Damping;
	this->K = -InitK;
}

Phy_Spring::~Phy_Spring()
{
}

glm::vec3 Phy_Spring::GetForce(Bod_Base P0)
{

	return K * (P0.GetPos() - Lock_Pos) - P0.GetVel() * Damping;
}

glm::vec3 Phy_Spring::GetForce(Bod_Base P0, Bod_Base P1)
{
	glm::vec3 RelV = P0.GetVel() - P1.GetVel();
	glm::vec3 Dir = P0.GetPos() - P1.GetPos();
	if (glm::dot(Dir, Dir))
		Dir = glm::normalize(Dir);
	float RelDis = glm::distance(P0.GetPos(),P1.GetPos());
	return K * (RelDis - 10) * Dir - RelV * Damping;
}

glm::vec3 CoatlPhysicsEngine::Phy_Spring::GetForce(Bod_Base P0, float dt)
{
	return glm::vec3(0.f);
}
