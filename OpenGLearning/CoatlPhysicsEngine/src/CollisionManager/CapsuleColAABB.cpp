#include "CapsuleColAABB.h"
using namespace CoatlPhysicsEngine;

bool CapsuleColAABB::CapColAABB(Capsule Cap, AABB_Obj Obj)
{
	std::vector<int> ID = Obj.GetSegmentsID();
	int Size = ID.size() / 2;
	glm::vec3 Pos = Obj.GetPos();
	std::vector<glm::vec3> Points = Obj.GetPoints();
	for (auto& jj : Points)
	{
		jj = jj + Pos;
	}
	int Index[] = { 0,4,1,5 };
	for (int ii = 0; ii < 4; ii++)
	{
	}

	int TopIndex[] = { 3,0,2,1 };
	return false;
}

CapsuleColAABB::CapsuleColAABB()
{
}

CapsuleColAABB::~CapsuleColAABB()
{
}
