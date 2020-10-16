#include "ContactConstraint.h"
#include <math.h>
using namespace CoatlPhysicsEngine;

std::vector<glm::vec3> ContactConstraint::U_Creation(glm::vec3 Norm, glm::vec3 DivVel)
{
	std::vector<glm::vec3> Us;
	glm::vec3 NormVel = DivVel * Norm * Norm;
	glm::vec3 TanVel = DivVel - NormVel;
	float T = TanVel.length();
	if (T > std::numeric_limits<float>::epsilon())
		Us.push_back(TanVel / T);
	else
		Us.push_back(glm::normalize(this->CreateOrtho(Norm)));
	Us.push_back(glm::cross(Norm, Us[0]));
	return Us;
}

void ContactConstraint::ResolveContact(std::shared_ptr<Contact> Cnt)
{
	if (Cnt->Bods[0]->GetParticle()->GetAwakeStatus())
	{
		//Contact Normals and other variables
		glm::vec3 Norm = Cnt->Normal;
		glm::vec3 Pos0 = Cnt->Bods[0]->GetPos();
		glm::vec3 Pos1 = Cnt->Bods[1]->GetPos();
		glm::vec3 R0 = Cnt->R0[0];
		glm::vec3 R1 = Cnt->R1[0];
		//Default Vels and Rots Needed for body 0
		glm::vec3 Vel0 = Cnt->Bods[0]->GetParticle()->GetVel();
		glm::vec3 RotVel0 = Cnt->Bods[0]->GetParticle()->GetRotVel();
		float InvMass0 = 1.f / Cnt->Bods[0]->GetParticle()->GetMass();
		glm::mat3 InvIner0 = glm::inverse(Cnt->Bods[0]->GetParticle()->GetInertiaWorld());
		//Calculate parts of the effecive Mass, DelVel and JV
		glm::vec3 N = glm::cross(R0, Norm);
		float M_E = InvMass0 + glm::dot(N * InvIner0,N);
		float JV = -glm::dot(Norm, Vel0) - glm::dot(N, RotVel0);
		glm::vec3 DelVel = -Vel0 - glm::cross(RotVel0,R0);
		if (Cnt->Bods[1]->GetParticle())
		{
			glm::vec3 Vel1 = Cnt->Bods[1]->GetParticle()->GetVel();
			glm::vec3 RotVel1 = Cnt->Bods[1]->GetParticle()->GetRotVel();
			float InvMass1 = 1.f / Cnt->Bods[1]->GetParticle()->GetMass();
			glm::mat3 InvIner1 = glm::inverse(Cnt->Bods[1]->GetParticle()->GetInertiaWorld());
			glm::vec3 M = glm::cross(R1, Norm);
			JV += glm::dot(Norm, Vel1) + glm::dot(N, RotVel1);
			M_E += InvMass1 + glm::dot(M * InvIner1,M);
			DelVel += Vel1 + glm::cross(RotVel1, R1);
		}
		//Find b, Lambda and set positions as well as U1 and U2
	}
}

glm::vec3 ContactConstraint::CreateOrtho(glm::vec3 Norm)
{
	std::vector<float> R;
	R.push_back(Norm.x);
	R.push_back(Norm.y);
	R.push_back(Norm.z);
	int ID = 0;
	for (int ii = 1; ii < 3; ii++)
	{
		if (glm::abs(R[ii]) < glm::abs(R[ID]))
			ID = ii;
	}
	float Den;
	if (ID == 0)
	{
		Den = glm::sqrt(Norm.y * Norm.y + Norm.z * Norm.z);
		return glm::vec3(0.f, -Norm.z / Den, Norm.y / Den);
	}
	else if (ID == 1)
	{
		Den = glm::sqrt(Norm.x * Norm.x + Norm.z * Norm.z);
		return glm::vec3(-Norm.z / Den,0.f, Norm.x / Den);
	}
	Den = glm::sqrt(Norm.x * Norm.x + Norm.y * Norm.y);
	return glm::vec3(-Norm.y / Den, Norm.x / Den, 0.f);
}

void ContactConstraint::SolveContactConstraint(std::vector<std::shared_ptr<Contact>> CntLst)
{
	for (auto& jj : CntLst)
	{
		this->ResolveContact(jj);
	}
}
