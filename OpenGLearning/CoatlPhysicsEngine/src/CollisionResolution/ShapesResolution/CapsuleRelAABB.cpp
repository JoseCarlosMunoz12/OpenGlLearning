#include "CapsuleRelAABB.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contact>> CapsuleRelAABB::CapRelAABB(Capsule Cap, AABB_Obj Obj)
{
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cnt = std::make_shared<Contact>();

	std::vector<glm::vec3> CapSegs = Cap.GetSegment();
	std::vector<glm::vec3> Obj_Segs = Obj.GetSegs();
	float R = Cap.GetRadius();

	std::vector<glm::vec3> Cap_N = {};
	std::vector<glm::vec3> Obj_N = { glm::normalize(Obj_Segs[1] - Obj_Segs[0]),
		glm::normalize(Obj_Segs[3] - Obj_Segs[0]) ,
		glm::normalize(Obj_Segs[4] - Obj_Segs[0]) };
	glm::vec3 Norm = glm::vec3(0.f,0.f,1.f);
	
	float Pntrtn = 0.f;
	Pntrtn = R - MATH::SATContact(Obj_N, Cap_N, Obj_Segs, CapSegs, Norm,R);	
	Cnt->Penetration = Pntrtn;
	Cnt->Normal = Norm;
	Cnt->ContactPoint;
	Temp.push_back(Cnt);
	return Temp;
}

CapsuleRelAABB::CapsuleRelAABB()
{
}

CapsuleRelAABB::~CapsuleRelAABB()
{
}
