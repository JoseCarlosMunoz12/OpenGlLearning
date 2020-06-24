#include "CapsuleRelCapsule.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contact>> CapsuleRelCapsule::CapRel(Capsule Cap0, Capsule Cap1)
{
	glm::vec3 C_P0;
	glm::vec3 C_P1;
	std::vector<glm::vec3> C_S0 = Cap0.GetSegment();
	std::vector<glm::vec3> C_S1 = Cap1.GetSegment();
	MATH::ClosestSeg_Seg(C_S0, C_S1, C_P0, C_P1);
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
		glm::vec3 C_N0 = C_S0[1] - C_S0[0];
		glm::vec3 C_N1 = C_S1[1] - C_S1[0];
		glm::vec3 N = glm::cross(C_N0, C_N1);
		float Mag = glm::dot(N, N);
		if (Mag == 0)
		{

		}
		else if (Mag == 1.f)
		{

		}
		else
		{

		}
	}
	return Temp;
}

CapsuleRelCapsule::CapsuleRelCapsule()
{

}

CapsuleRelCapsule::~CapsuleRelCapsule()
{
}
