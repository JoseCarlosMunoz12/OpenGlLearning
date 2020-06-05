#include "CapsuleRelAABB.h"
using namespace CoatlPhysicsEngine;
int CapsuleRelAABB::line_support(glm::vec3& Support, glm::vec3 D, Capsule Cap)
{	
	std::vector<glm::vec3> AB = Cap.GetSegment();
	if (glm::dot(AB[0], D) < glm::dot(AB[1], D))
	{
		Support = AB[0];
		return 0;
	}
	Support = AB[1];
	return 0;
}
int CoatlPhysicsEngine::CapsuleRelAABB::poly_support(glm::vec3& Support, glm::vec3 D, AABB_Obj obj)
{
	int IMax = 0;
	std::vector<glm::vec3> Vecs = obj.GetSegs();
	float dMax = glm::dot(Vecs[0], D);
	int Count = 0;
	for (auto& j : Vecs)
	{
		float dot = glm::dot(j, D);
		if (dot < dMax)
		{
			IMax = Count;
			dMax = dot;
			Support = j;
		}
		Count++;
	}
	return IMax;
}
std::vector<std::shared_ptr<Contact>> CapsuleRelAABB::CapRelAABB(Capsule Cap, AABB_Obj Obj)
{
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cnt = std::make_shared<Contact>();

	std::vector<glm::vec3> CapSegs = Cap.GetSegment();
	std::vector<glm::vec3> Obj_Segs = Obj.GetSegs();
	float R = Cap.GetRadius();

	std::vector<glm::vec3> Cap_N = { glm::normalize(CapSegs[1] - CapSegs[0]) };
	std::vector<glm::vec3> Obj_N = { glm::normalize(Obj_Segs[1] - Obj_Segs[0]),
		glm::normalize(Obj_Segs[3] - Obj_Segs[0]) ,
		glm::normalize(Obj_Segs[4] - Obj_Segs[0]) };
	glm::vec3 Norm = glm::vec3(0.f,0.f,1.f);
	
	float Pntrtn = 0.f;
	if (MATH::GJK_Algorithm(CapSegs, Obj_Segs))
		Pntrtn = MATH::SATContact(Obj_N, Cap_N, Obj_Segs, CapSegs, Norm);
	if (Pntrtn < R)
		Pntrtn = R - Pntrtn;
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
