#include "TriangleColAABB.h"
using namespace CoatlPhysicsEngine;

bool TriangleColAABB::TrColAABB(Triangles Tr, AABB_Obj AABB)
{
	float P0, P1, P2, R;

	glm::vec3 C = AABB.GetPos();
	glm::vec3 Ex = AABB.GetLengths();
	glm::vec3 TrPos = Tr.GetPos();
	std::vector<glm::vec3> TrPnt = Tr.GetSegments();

	glm::vec3 V0 = (TrPnt[0] + TrPos) - C;
	glm::vec3 V1 = (TrPnt[1] + TrPos) - C;
	glm::vec3 V2 = (TrPnt[2] + TrPos) - C;
	
	glm::vec3 F0 = V1 - V0;
	glm::vec3 F1 = V2 - V1;
	glm::vec3 F2 = V0 - V2;
	
	P0 = V0.z * V1.y - V0.y * V1.z;
	P2 = V2.z * (V1.y - V0.y) - V2.y * (V1.z - V0.z);
	R = Ex.y * glm::abs(F0.z) + Ex.z * glm::abs(F0.y);

	if (glm::max(-glm::max(P0, P2), glm::min(P0,P2)) > R)
	{
		return true;
	}


	return false;
}

TriangleColAABB::TriangleColAABB()
{
}

TriangleColAABB::~TriangleColAABB()
{
}
