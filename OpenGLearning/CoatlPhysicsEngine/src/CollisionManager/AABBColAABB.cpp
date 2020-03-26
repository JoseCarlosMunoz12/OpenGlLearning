#include "AABBColAABB.h"
using namespace CoatlPhysicsEngine;

bool AABBColAABB::AABBAABB(AABB_Obj Obj0, AABB_Obj Obj1)
{
	glm::vec3 A = Obj0.GetLengths();
	glm::vec3 A_Pos = Obj0.GetPos();
	glm::vec3 B = Obj1.GetLengths();
	glm::vec3 B_Pos = Obj1.GetPos();

	int R;
	R = A.x + B.x;
	if ((unsigned int)(A_Pos.x - B_Pos.x + R) > ( R + R ))
	{
		return true;
	}
	R = A.y + B.y;
	if ((unsigned int)(A_Pos.y - B_Pos.y + R) > (R + R))
	{
		return true;
	}
	R = A.z + B.z;
	if ((unsigned int)(A_Pos.z - B_Pos.z + R) > (R + R))
	{
		return true;
	}
	return false;
}

AABBColAABB::AABBColAABB()
{

}

AABBColAABB::~AABBColAABB()
{
}
