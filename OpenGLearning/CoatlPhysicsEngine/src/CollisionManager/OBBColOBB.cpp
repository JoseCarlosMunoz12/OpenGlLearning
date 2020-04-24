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
	for (auto& jj : Ob0_N)
	{
		if (MATH::ProjColl(jj, Ob0_Segs, Ob1_Segs))
		{
			return false;
		}
	}
	for (auto& jj : Ob1_N)
	{
		if (MATH::ProjColl(jj, Ob0_Segs, Ob1_Segs))
		{
			return false;
		}
	}

	return true;
}

OBBColOBB::OBBColOBB()
{
}

OBBColOBB::~OBBColOBB()
{
}
