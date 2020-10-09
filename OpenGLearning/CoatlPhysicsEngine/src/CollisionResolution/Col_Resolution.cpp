#include "Col_Resolution.h"
using namespace CoatlPhysicsEngine;

Col_Resolution::Col_Resolution(int InitVel, int InitPos)
	:VelIt(InitVel), Posit(InitPos)
{
}

Col_Resolution::~Col_Resolution()
{
}

void Col_Resolution::ResolveContacts(std::vector<std::shared_ptr<Contacts>> Cnt,float dt)
{
	for (auto& jj : Cnt)
	{
		glm::vec3 Vec0 = jj->Bods[0]->GetParticle()->GetVel();
		glm::vec3 Pos0 = jj->Bods[0]->GetPos() + Vec0 * jj->dt0;
		glm::vec3 AngVel = jj->Bods[0]->GetParticle()->GetRotVel();
		glm::quat Quat = jj->Bods[0]->GetQuat();
		glm::vec3 Pos1 = Cnt[0]->Bods[1]->GetPos();
		glm::vec3 C = (jj->R1[0] + Pos1) - (jj->R0[0] + Pos0);
		float CN = glm::dot(C, jj->Normal);
		float B = 0.3f;
		float R = jj->Restituion * glm::dot(-Vec0 - glm::cross(AngVel,jj->R0[0]), jj->Normal);
		Vec0.z = R + (B / jj->dt1) * CN;
		jj->Bods[0]->GetParticle()->SetVel(Vec0);
		Pos0 += jj->dt1 * Vec0;
		jj->Bods[0]->SetPosition(Pos0);
	}
}

/////////////////////////////////////////////////////////////////////////
//----------------------------Contact class----------------------------//
/////////////////////////////////////////////////////////////////////////