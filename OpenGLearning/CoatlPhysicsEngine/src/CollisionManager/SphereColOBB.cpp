#include "SphereColOBB.h"
using namespace CoatlPhysicsEngine;

bool SphereColOBB::SphColOBB(Sphere Sph, OBB Obj)
{
	glm::vec3 ClsPoint = Obj.GetClosestPoint(Sph.GetPos());
	float Rad = Sph.GetRadius();
	glm::vec3 V = ClsPoint - Sph.GetPos();
	float Dot = glm::dot(V, V);
	Rad = Rad * Rad;
	return Dot <= Rad;
}

SphereColOBB::SphereColOBB()
{
}

SphereColOBB::~SphereColOBB()
{
}
