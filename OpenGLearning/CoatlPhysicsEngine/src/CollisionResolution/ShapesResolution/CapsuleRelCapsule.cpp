#include "CapsuleRelCapsule.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contacts>> CapsuleRelCapsule::CapRel(Capsule Cap0, Capsule Cap1)
{
	glm::vec3 C_P0;
	glm::vec3 C_P1;
	std::vector<glm::vec3> C_S0 = Cap0.GetSegment();
	std::vector<glm::vec3> C_S1 = Cap1.GetSegment();
	glm::vec3 Dir0 = MATH::CreateNormal(C_S0);
	glm::vec3 Dir1 = MATH::CreateNormal(C_S1);
	MATH::ClosestSeg_Seg(C_S0, C_S1, C_P0, C_P1);
	std::vector<std::shared_ptr<Contacts>> Temp;
	std::shared_ptr<Contacts> Con = std::make_shared<Contacts>();
	glm::vec3 Norm = C_P1 - C_P0;
	if (glm::length(Norm) > 0)
		Norm = -Norm / glm::length(Norm);
	std::vector<glm::vec3> Cont;
	std::vector<glm::vec3> PL0;
	std::vector<glm::vec3> PL1;
	if (Norm != glm::vec3(0.f))
	{
		float S = glm::dot(Norm, Dir0);
		float T = glm::dot(Norm, Dir1);
		if (S == 0.f && T == 0.f && glm::cross(Dir0, Dir1) == glm::vec3(0.f))
		{
			Cont = MATH::SAT_Points(Dir0, C_S0, C_S1);
			PL0 = MATH::SAT_Points(Norm, C_S0, C_S1);
			PL1 = MATH::SAT_Points(glm::cross(Dir0, Norm), C_S0, C_S1);
			float Total_R = Cap0.GetRadius() + Cap1.GetRadius();
			float dis = glm::distance(C_P0, C_P1);
			std::shared_ptr<Contacts> Contact = std::make_shared<Contacts>();
			Contact->Normal = Norm;
			Contact->Penetration = Total_R - dis;
			for (auto& ii : Cont)
			{
				glm::vec3 R = (PL0[0] + PL0[1]) / 2.f;
				glm::vec3 V = (PL1[0] + PL1[1]) / 2.f;
				Contact->ContactPoint.push_back(ii + R + V);
			}
			Contact->R0.push_back(Cont[0] + PL0[0] + PL1[0] - Norm * Cap0.GetRadius() - Cap0.GetPos());
			Contact->R1.push_back(Cont[1] + PL0[1] + PL1[1] - Norm * Cap1.GetRadius() - Cap1.GetPos());
			Temp.push_back(Contact);
		}
		else
		{
			float Total_R = Cap0.GetRadius() + Cap1.GetRadius();
			float dis = glm::distance(C_P0, C_P1);
			Con->Penetration = Total_R - dis;
			Con->Normal = Norm;
			Con->ContactPoint.push_back(C_P0 + Norm * Con->Penetration);
			Con->R0.push_back(C_P0 - Norm * Cap0.GetRadius() - Cap0.GetPos());
			Con->R1.push_back(C_P1 + Norm * Cap1.GetRadius() - Cap1.GetPos());
			Temp.push_back(Con);
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
