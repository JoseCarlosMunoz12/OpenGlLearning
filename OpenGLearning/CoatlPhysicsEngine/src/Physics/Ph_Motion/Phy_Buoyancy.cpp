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

glm::vec3 Phy_Buoyancy::GetForce(Bod_Base P0)
{	
	glm::vec3 Grav = glm::vec3(0.f, 0.f,9.81f);
	glm::vec3 Pos =  P0.GetPos() - glm::vec3(0.f,0.f,W_Floor);

	return 10 * W_Density * Grav;
	
}

glm::vec3 Phy_Buoyancy::GetForce(Bod_Base P0, Bod_Base P1)
{	
	return glm::vec3();
}

glm::vec3 Phy_Buoyancy::GetForce(Bod_Base P0, float dt)
{
	return glm::vec3();
}
