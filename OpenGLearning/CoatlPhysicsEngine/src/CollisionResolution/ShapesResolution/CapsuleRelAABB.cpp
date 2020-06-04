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
std::vector<std::shared_ptr<Contact>> CapsuleRelAABB::CapRelAABB(Capsule Cap, AABB_Obj Obj)
{
	std::shared_ptr<gjk_support> s = std::make_shared<gjk_support>();
	s->A = Obj.GetSegs()[0];
	s->B = Cap.GetSegment()[0];
	s->D = s->B - s->A;
	std::shared_ptr<gjk_simplex> gsx =std::make_shared<gjk_simplex>();
	while (GJK(gsx,s))
	{
		s->BId = line_support(s->B, s->DB, Cap);
	}
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cnt = std::make_shared<Contact>();
	if (Dist != 0)
	{
		Cnt->Penetration;
		Cnt->Normal;
	}
	else
	{
		std::vector<glm::vec3> ABB_Segs = Cap.GetSegment();
		std::vector<glm::vec3> Obj_Segs = Obj.GetSegs();
		float R = Cap.GetRadius();

		std::vector<glm::vec3> ABB_N = { glm::normalize(ABB_Segs[1] - ABB_Segs[0]) };
		std::vector<glm::vec3> Obj_N = { glm::normalize(Obj_Segs[1] - Obj_Segs[0]),
			glm::normalize(Obj_Segs[3] - Obj_Segs[0]) ,
			glm::normalize(Obj_Segs[4] - Obj_Segs[0]) };
		glm::vec3 Norm;
		Cnt->Penetration = MATH::SATContact(ABB_N, Obj_N, ABB_Segs, Obj_Segs, Norm) + R;
		Cnt->Normal = Norm;
	}
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
