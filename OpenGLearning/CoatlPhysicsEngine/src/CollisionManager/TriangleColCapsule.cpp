#include "TriangleColCapsule.h"
using namespace CoatlPhysicsEngine;

bool TriangleColCapsule::TrColCap(Triangles Tr, Capsule Cap)
{
	std::vector<glm::vec3> TrPoints = Tr.GetSegments();
	std::vector<glm::vec3> CapPoints = Cap.GetSegment();
	glm::vec3 TrPos = Tr.GetPos();

	float DisAB = MATH::Distance_Seg({ TrPoints[0],TrPoints[1] }, TrPos, CapPoints);
	float DisBC = MATH::Distance_Seg({ TrPoints[1],TrPoints[2] }, TrPos, CapPoints);
	float DisCA = MATH::Distance_Seg({ TrPoints[2],TrPoints[0] }, TrPos, CapPoints);
	float PntATr = MATH::Distance_Tr_Pnt(TrPoints, TrPos, CapPoints[0]);
	float PntBTr = MATH::Distance_Tr_Pnt(TrPoints, TrPos, CapPoints[1]);

	return false;
}

TriangleColCapsule::TriangleColCapsule()
{
}

TriangleColCapsule::~TriangleColCapsule()
{
}
