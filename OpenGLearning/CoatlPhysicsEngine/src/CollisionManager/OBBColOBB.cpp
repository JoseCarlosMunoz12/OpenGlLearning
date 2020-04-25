#include "OBBColOBB.h"
using namespace CoatlPhysicsEngine;

bool OBBColOBB::OBBCol(OBB Ob0, OBB Ob1)
{
	std::vector<glm::vec3> Ob0_Segs = Ob0.GetSegments();
	std::vector<glm::vec3> Ob1_Segs = Ob1.GetSegments();

	std::vector<glm::vec3> Ob0_N = { glm::normalize(Ob0_Segs[1] - Ob0_Segs[0]),
		glm::normalize(Ob0_Segs[3] - Ob0_Segs[0]) ,
		glm::normalize(Ob0_Segs[4] - Ob0_Segs[0]) };
	std::vector<glm::vec3> Ob1_N = { glm::normalize(Ob1_Segs[1] - Ob1_Segs[0]),
		glm::normalize(Ob1_Segs[3] - Ob1_Segs[0]) ,
		glm::normalize(Ob1_Segs[4] - Ob1_Segs[0]) };
	return MATH::SATColCheck(Ob0_N, Ob1_N, Ob0_Segs, Ob1_Segs);
}

OBBColOBB::OBBColOBB()
{
}

OBBColOBB::~OBBColOBB()
{
}
