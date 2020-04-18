#include "TriangleColAABB.h"
using namespace CoatlPhysicsEngine;

bool TriangleColAABB::TrColAABB(Triangles Tr, AABB_Obj AABB)
{
	glm::vec3 C = AABB.GetPos();
	glm::vec3 Ex = AABB.GetLengths();
	Tr.GetSegments();
	
	return false;
}

TriangleColAABB::TriangleColAABB()
{
}

TriangleColAABB::~TriangleColAABB()
{
}
