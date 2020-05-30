#include "CapsuleRelAABB.h"
using namespace CoatlPhysicsEngine;
std::vector<std::shared_ptr<Contact>> CapsuleRelAABB::CapRelAABB(Capsule Cap, AABB_Obj Obj)
{
	float R = Cap.GetRadius() /2;
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cnt = std::make_shared<Contact>();
	std::vector<glm::vec3> ABB_Segs = Cap.GetSegment();
	std::vector<glm::vec3> Obj_Segs = Obj.GetSegs();

	std::vector<glm::vec3> ABB_N = {glm::normalize(ABB_Segs[1] - ABB_Segs[0])};
	std::vector<glm::vec3> Obj_N = {glm::normalize(Obj_Segs[1] - Obj_Segs[0]),
	glm::normalize(Obj_Segs[3] - Obj_Segs[0]),
	glm::normalize(Obj_Segs[4] - Obj_Segs[0])};

	glm::vec3 Norm;

	Cnt->Penetration = MATH::SATContact(ABB_N, Obj_N, ABB_Segs, Obj_Segs, Norm) + R ;
	Cnt->Normal = Norm;
	Cnt->ContactPoint = Cnt->Penetration * Cnt->Normal;

	Temp.push_back(Cnt);
	return Temp;
}

CapsuleRelAABB::CapsuleRelAABB()
{
}

CapsuleRelAABB::~CapsuleRelAABB()
{
}
