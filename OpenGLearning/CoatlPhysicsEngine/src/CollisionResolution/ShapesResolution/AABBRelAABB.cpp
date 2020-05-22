#include "AABBRelAABB.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contact>> AABBRelAABB::AABB_Rel(AABB_Obj Obj0, AABB_Obj Obj1)
{
	//Possible NormalVectors
	static const glm::vec3 Faces[6] = {
	glm::vec3(-1.f, 0.f, 0.f) ,glm::vec3( 1.f, 0.f, 0.f),
	glm::vec3( 0.f,-1.f, 0.f) ,glm::vec3( 0.f, 1.f, 0.f),
	glm::vec3( 0.f, 0.f,-1.f) ,glm::vec3( 0.f, 0.f, 1.f)};
	//Get max and Mins
	glm::vec3 MaxA = Obj0.GetPos() + Obj0.GetEx();
	glm::vec3 MinA = Obj0.GetPos() - Obj0.GetEx();

	glm::vec3 MaxB = Obj1.GetPos() + Obj1.GetEx();
	glm::vec3 MinB = Obj1.GetPos() - Obj1.GetEx();
	// Distances From each other
	float Dis[6] = {(MaxB.x - MinA.x) ,
					(MaxA.x - MinB.x) ,
					(MaxB.y - MinA.y) ,
					(MaxA.y - MinB.y) ,
					(MaxB.z - MinA.z) ,
					(MaxA.z - MinB.z)};
	float Penetration = Dis[0];
	glm::vec3 Norm = Faces[0];
	for (int ii = 0; ii < 6; ii++)
	{
		if (Dis[ii] < Penetration)
		{
			Penetration = Dis[ii];
			Norm = Faces[ii];
		}
	}

	return std::vector<std::shared_ptr<Contact>>();
}

AABBRelAABB::AABBRelAABB()
{
}

AABBRelAABB::~AABBRelAABB()
{
}
