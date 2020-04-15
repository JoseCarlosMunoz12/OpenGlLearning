#include "TriangleColCapsule.h"
#include <algorithm>
using namespace CoatlPhysicsEngine;

bool TriangleColCapsule::TrColCap(Triangles Tr, Capsule Cap)
{
	std::vector<glm::vec3> TrPoints = Tr.GetSegments();
	std::vector<glm::vec3> CapPoints = Cap.GetSegment();
	float R = Cap.GetRadius();
	glm::vec3 TrPos = Tr.GetPos();
	std::vector<float> DisVals;
	DisVals.push_back(MATH::Distance_Seg({ TrPoints[0],TrPoints[1] }, TrPos, CapPoints));
	DisVals.push_back(MATH::Distance_Seg({ TrPoints[1],TrPoints[2] }, TrPos, CapPoints));
	DisVals.push_back(MATH::Distance_Seg({ TrPoints[2],TrPoints[0] }, TrPos, CapPoints));
	DisVals.push_back(MATH::Distance_Tr_Pnt(TrPoints, TrPos, CapPoints[0]));
	DisVals.push_back(MATH::Distance_Tr_Pnt(TrPoints, TrPos, CapPoints[1]));
	
	float MinVal = *std::min_element(DisVals.begin(), DisVals.end());
	return  R > MinVal;
}

TriangleColCapsule::TriangleColCapsule()
{
}

TriangleColCapsule::~TriangleColCapsule()
{
}
