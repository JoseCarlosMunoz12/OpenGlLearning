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


	return false;
}
