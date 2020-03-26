#include "AABBColAABB.h"
using namespace CoatlPhysicsEngine;

bool AABBColAABB::AABBAABB(AABB_Obj Obj0, AABB_Obj Obj1)
{
	glm::vec3 A = Obj0.GetLengths();
	glm::vec3 A_Pos = Obj0.GetPos();
	glm::vec3 B = Obj1.GetLengths();
	glm::vec3 B_Pos = Obj1.GetPos();
	if (glm::abs(A_Pos.x - B_Pos.x) > (A.x + B.x))
	{
		return false;
	}
	if (glm::abs(A_Pos.y - B_Pos.y) > (A.y + B.y))
	{
		return false;
	}
	if (glm::abs(A_Pos.z - B_Pos.z) > (A.z + B.z))
	{
		return false;
	}
	return true;
}

AABBColAABB::AABBColAABB()
{

}

AABBColAABB::~AABBColAABB()
{
}
