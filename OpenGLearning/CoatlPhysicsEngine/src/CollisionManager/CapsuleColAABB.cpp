#include "CapsuleColAABB.h"
using namespace CoatlPhysicsEngine;

bool CapsuleColAABB::CapColAABB(Capsule Cap, AABB_Obj Obj)
{
	std::vector<int> ID = Obj.GetSegmentsID();
	int Size = ID.size() / 2;
	glm::vec3 Pos = Obj.GetPos();
	std::vector<glm::vec3> Points = Obj.GetSegs();
	float R = Cap.GetRadius();
	glm::vec3 Pos0;
	glm::vec3 Pos1;
	MATH::ClosestSeg_Seg(Cap.GetSegment(), { Points[ID[0]], Points[ID[1]] },Pos0,Pos1);
	glm::vec3 Cls_Pnt = Pos0;
	float TempDis = glm::distance(Pos0, Pos1);
	float Dis;
	for (int ii = 0; ii < Size; ii++)
	{
		int JJ = ii * 2;
		int KK = JJ + 1;
		MATH::ClosestSeg_Seg(Cap.GetSegment(), { Points[ID[JJ]], Points[ID[KK]] }, Pos0, Pos1);
		Dis = glm::distance(Pos0, Pos1);
		if (Dis < TempDis)
		{
			Dis = TempDis;
			Cls_Pnt = Pos0;
		}
	}
	glm::vec3 ClsPoint = Obj.GetClosesPoint(Cls_Pnt);
	float Rad = Cap.GetRadius();
	float Dot = glm::distance(ClsPoint, Cls_Pnt);
	return  Dot <= Rad;

}

CapsuleColAABB::CapsuleColAABB()
{
}

CapsuleColAABB::~CapsuleColAABB()
{
}
