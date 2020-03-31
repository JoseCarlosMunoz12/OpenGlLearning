#include "SphereColAABB.h"
using namespace CoatlPhysicsEngine;

CoatlPhysicsEngine::SphereColAABB::SphereColAABB()
{
}

CoatlPhysicsEngine::SphereColAABB::~SphereColAABB()
{
}

bool CoatlPhysicsEngine::SphereColAABB::SphereColsAABB(Sphere Sph0, AABB_Obj Obj0)
{
	glm::vec3 ClsPoint = Obj0.GetClosesPoint(Sph0.GetPos());
	float Rad = Sph0.GetRadius();
	glm::vec3 V = ClsPoint - Sph0.GetPos();
	float Dot = V.x * V.x + V.y * V.y+ V.z * V.z;
	Rad = Rad * Rad;
	return  Dot <= Rad;
}
