#include "TriangleColOBB.h"
using namespace CoatlPhysicsEngine;

bool TriangleColOBB::TriColOBB(Triangles Tr, OBB Obj)
{
	//Get Needed Info
	glm::vec3 TrPos = Tr.GetPos();
	std::vector<glm::vec3> TrSeg = Tr.GetSegments();
	for (auto& ii : TrSeg)
	{
		ii = ii + TrPos;
	}
	std::vector<glm::vec3> ObjSeg = Obj.GetSegments();	
	std::vector<glm::vec3> Obj_N = { glm::normalize(ObjSeg[1] - ObjSeg[0]),
	glm::normalize(ObjSeg[3] - ObjSeg[0]) ,
	glm::normalize(ObjSeg[4] - ObjSeg[0]) };
	//Check if Points are not in the plane
	C_Plane TrPlane(TrSeg);
	if (TrPlane.SameLocPl(ObjSeg))
	{
		return false;
	}
	// SAT Test
	
	return MATH::SATColCheck({ TrPlane.Normal }, Obj_N, TrSeg, ObjSeg);
}

TriangleColOBB::TriangleColOBB()
{
}

TriangleColOBB::~TriangleColOBB()
{
}
