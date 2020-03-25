#include "SphereColSphere.h"
using namespace CoatlPhysicsEngine;
SphereColSphere::SphereColSphere()
{
}

SphereColSphere::~SphereColSphere()
{
}

bool SphereColSphere::SphereSphere(Sphere Sphere0,Sphere Sphere1)
{
	float R_Total = Sphere0.GetRadius() + Sphere1.GetRadius();
	R_Total = R_Total * R_Total;
	glm::vec3 Vec_Total = Sphere0.GetPos() - Sphere1.GetPos();
	float Mid_Dis = Vec_Total.x * Vec_Total.x + Vec_Total.y * Vec_Total.y + Vec_Total.z * Vec_Total.z;
	return R_Total > Mid_Dis;
}
