#include "SphereColAABB.h"
using namespace CoatlPhysicsEngine;

SphereColAABB::SphereColAABB()
{
}

SphereColAABB::~SphereColAABB()
{
}

bool SphereColAABB::SphereColsAABB(Sphere Sph0, AABB_Obj Obj0)
{	
	glm::vec3 ClsPoint = Obj0.GetClosesPoint(Sph0.GetPos());
	float Rad = Sph0.GetRadius();
	glm::vec3 V = ClsPoint - Sph0.GetPos();
	float Dot = glm::dot(V,V);
	Rad = Rad * Rad;
	return  Dot <= Rad;
}
