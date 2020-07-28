#include "CapsuleRelCapsule.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contact>> CapsuleRelCapsule::CapRel(Capsule Cap0, Capsule Cap1)
{
	glm::vec3 C_P0;
	glm::vec3 C_P1;
	std::vector<glm::vec3> C_S0 = Cap0.GetSegment();
	std::vector<glm::vec3> C_S1 = Cap1.GetSegment();
	glm::vec3 Dir0 = MATH::CreateNormal(C_S0);
	glm::vec3 Dir1 = MATH::CreateNormal(C_S1);
	MATH::ClosestSeg_Seg(C_S0, C_S1, C_P0, C_P1);
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Con = std::make_shared<Contact>();
	glm::vec3 Norm = C_P1 - C_P0;
	Norm = MATH::Normalize(Norm);
	if (Norm != glm::vec3(0.f))
	{
		float S = glm::dot(Norm, Dir0);
		float T = glm::dot(Norm, Dir1);
		if (S == 0.f && T == 0.f)
		{

		}
	}
	float Total_R = Cap0.GetRadius() + Cap1.GetRadius();
	float dis = glm::distance(C_P0, C_P1);
	Con->Penetration = Total_R - dis;
	Con->Normal = -Norm;
	Con->ContactPoint = C_P0 - Norm * Con->Penetration;
	Temp.push_back(Con);
	return Temp;
}

CapsuleRelCapsule::CapsuleRelCapsule()
{

}

CapsuleRelCapsule::~CapsuleRelCapsule()
{
}
