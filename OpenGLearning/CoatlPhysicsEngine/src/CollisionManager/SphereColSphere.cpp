#include "SphereColSphere.h"
using namespace CoatlPhysicsEngine;
SphereColSphere::SphereColSphere()
{
}

SphereColSphere::~SphereColSphere()
{
}

bool CoatlPhysicsEngine::SphereColSphere::Collided(glm::vec3 Pos0, Sphere Sphere0, glm::vec3 Pos1, Sphere Sphere1)
{
	float R_Total = Sphere0.GetRadius() + Sphere1.GetRadius();
	R_Total *= R_Total;
	glm::vec3 Vec_Total = Pos1 - Pos0;
	float Mid_Dis = Vec_Total.x * Vec_Total.x + Vec_Total.y * Vec_Total.y + Vec_Total.z * Vec_Total.z;
	return R_Total > Mid_Dis;
}
