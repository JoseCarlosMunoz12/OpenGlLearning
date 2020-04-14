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
	float R = Cap.GetRadius();
	for (int ii = 0; ii < Size; ii++)
	{
		int JJ = ii * 2;
		int KK = JJ + 1;
		float Dis = MATH::Distance_Seg(Cap.GetSegment(), {Points[ID[JJ]], Points[ID[KK]]});
		if (R > Dis)
		{
			return true;
		}
	}
	return false;
}

CapsuleColAABB::CapsuleColAABB()
{
}

CapsuleColAABB::~CapsuleColAABB()
{
}
