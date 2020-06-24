#include "CapsuleRelCapsule.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contact>> CapsuleRelCapsule::CapRel(Capsule Cap0, Capsule Cap1)
{
	glm::vec3 C_P0;
	glm::vec3 C_P1;
	MATH::ClosestSeg_Seg(Cap0.GetSegment(), Cap1.GetSegment(), C_P0, C_P1);
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Con = std::make_shared<Contact>();
	glm::vec3 Norm = C_P1 - C_P0;
	if (glm::dot(Norm, Norm) != 0.f)
	{
		Norm = glm::normalize(Norm);
		float Total_R = Cap0.GetRadius() + Cap1.GetRadius();
		float dis = glm::distance(C_P0, C_P1);
		Con->Penetration = Total_R - dis;
		Con->Normal = -Norm;
		Con->ContactPoint = C_P0 - Norm * Con->Penetration;
		Temp.push_back(Con);
	}
	else
	{

	}
	return Temp;
}

CapsuleRelCapsule::CapsuleRelCapsule()
{

}

CapsuleRelCapsule::~CapsuleRelCapsule()
{
}
