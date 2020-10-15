#include "Col_Resolution.h"
using namespace CoatlPhysicsEngine;

Col_Resolution::Col_Resolution(int InitVel, int InitPos)
	:VelIt(InitVel), Posit(InitPos)
{
}

Col_Resolution::~Col_Resolution()
{
}

void Col_Resolution::ResolveContacts(std::vector<std::shared_ptr<Contact>> Cnt,float dt)
{
	for (auto& jj : Cnt)
		if(jj->Bods[0]->GetParticle()->GetAwakeStatus())
		{
			glm::vec3 Vec0 = jj->Bods[0]->GetParticle()->GetVel();
			float Mass0 = 1 / jj->Bods[0]->GetParticle()->GetMass();
			glm::mat3 InvInert = glm::inverse(jj->Bods[0]->GetParticle()->GetInertiaWorld());
			glm::vec3 Pos0 = jj->Bods[0]->GetPos() + Vec0 * jj->dt0;
			glm::vec3 AngVel = jj->Bods[0]->GetParticle()->GetRotVel();
			glm::quat Quat = jj->Bods[0]->GetQuat();
			glm::quat E(0,AngVel * jj->dt0);
			Quat = Quat + .5f * Quat * E;
			Quat = glm::normalize(Quat);
			glm::vec3 Pos1 = Cnt[0]->Bods[1]->GetPos();
			//Calculate values of Lambda, C and b
			glm::vec3 C = (jj->R1[0] + Pos1) - (jj->R0[0] + Pos0);
			float CN = glm::dot(C, jj->Normal);
			float B = 0.3f;
			float b = jj->Restituion * glm::dot(-Vec0 - glm::cross(AngVel,jj->R0[0]), jj->Normal) + (B / jj->dt1) * CN;
			float lambda = glm::pow(Mass0 + glm::dot(glm::cross(jj->R0[0], jj->Normal) * InvInert, glm::cross(jj->R0[0],jj->Normal)),-1) * b;
			//Calculate new Vel and AngVel
			Vec0 = Vec0 + Mass0 * jj->Normal * lambda;
			AngVel +=  InvInert * glm::cross(jj->R0[0], jj->Normal) * -1.f * lambda;
			//Setting new pos  and ang
			jj->Bods[0]->GetParticle()->SetVel(Vec0);
			jj->Bods[0]->GetParticle()->SetRotVel(AngVel);
			E = glm::quat(0, AngVel * jj->dt1);
			Quat = Quat + .5f * Quat * E;
			Quat = glm::normalize(Quat);
			Pos0 += jj->dt1 * Vec0;
			jj->Bods[0]->SetQuat(Quat);
			jj->Bods[0]->SetPosition(Pos0);
		}
}

/////////////////////////////////////////////////////////////////////////
//----------------------------Contact class----------------------------//
/////////////////////////////////////////////////////////////////////////