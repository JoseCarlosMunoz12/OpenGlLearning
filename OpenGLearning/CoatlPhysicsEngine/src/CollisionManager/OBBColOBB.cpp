#include "OBBColOBB.h"
using namespace CoatlPhysicsEngine;

bool OBBColOBB::OBBCol(OBB Ob0, OBB Ob1)
{
	std::vector<glm::vec3> Ob0_Segs = Ob0.GetSegments();
	std::vector<glm::vec3> Ob1_Segs = Ob1.GetSegments();

	std::vector<glm::vec3> OB0_N;
	std::vector<glm::vec3> OB1_N;
	for (auto& jj : Ob0_Segs)
	{
		if (MATH::ProjColl(jj, Ob0_Segs, Ob1_Segs))
		{
			return false;
		}
	}
	for (auto& jj : Ob1_Segs)
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
