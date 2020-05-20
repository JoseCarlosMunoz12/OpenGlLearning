#include "Phy_Buoyancy.h"
using namespace CoatlPhysicsEngine;
Phy_Buoyancy::Phy_Buoyancy(float Floor, float MaxDepth, float Height, float Density)
{
	this->W_Floor = Floor;
	this->W_Max_Depth = MaxDepth;
	this->W_Height = Height;
	this->W_Density = Density;
}

Phy_Buoyancy::~Phy_Buoyancy()
{
}

glm::vec3 Phy_Buoyancy::GetForce(Particle P0)
{
	return glm::vec3(0.f);
	
}

glm::vec3 Phy_Buoyancy::GetForce(Particle P0, Particle P1)
{
	return glm::vec3();
}

glm::vec3 Phy_Buoyancy::GetForce(Particle P0, float dt)
{
	return glm::vec3();
}
