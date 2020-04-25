#include "ABBColOBB.h"
using namespace CoatlPhysicsEngine;
bool ABBColOBB::ABBColsOBB(AABB_Obj ABB, OBB Obj)
{
	std::vector<glm::vec3> ABB_Segs = ABB.GetSegs();
	std::vector<glm::vec3> Obj_Segs = Obj.GetSegments();

	std::vector<glm::vec3> ABB_N = { glm::normalize(ABB_Segs[1] - ABB_Segs[0]),
		glm::normalize(ABB_Segs[3] - ABB_Segs[0]) ,
		glm::normalize(ABB_Segs[4] - ABB_Segs[0]) };
	std::vector<glm::vec3> Obj_N = { glm::normalize(Obj_Segs[1] - Obj_Segs[0]),
		glm::normalize(Obj_Segs[3] - Obj_Segs[0]) ,
		glm::normalize(Obj_Segs[4] - Obj_Segs[0]) };
	return MATH::SATColCheck(ABB_N, Obj_N, ABB_Segs, Obj_Segs);
}

ABBColOBB::ABBColOBB()
{
}

ABBColOBB::~ABBColOBB()
{
}
