#include "CapsuleRelAABB.h"
using namespace CoatlPhysicsEngine;
std::vector<std::shared_ptr<Contact>> CapsuleRelAABB::CapRelAABB(Capsule Cap, AABB_Obj Obj)
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
	glm::vec3 Pos0;
	glm::vec3 Pos1;
	MATH::ClosestSeg_Seg(Cap.GetSegment(), { Points[ID[0]], Points[ID[1]] }, Pos0, Pos1);
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
	float Dist = glm::distance(ClsPoint, Cls_Pnt);
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cnt = std::make_shared<Contact>();
	Cnt->Penetration = R - Dist;
	Cnt->Normal = -glm::normalize(ClsPoint - Cls_Pnt);
	Cnt->ContactPoint = ClsPoint + Cnt->Penetration * Cnt->Normal;
	Temp.push_back(Cnt);
	return Temp;
}

CapsuleRelAABB::CapsuleRelAABB()
{
}

CapsuleRelAABB::~CapsuleRelAABB()
{
}