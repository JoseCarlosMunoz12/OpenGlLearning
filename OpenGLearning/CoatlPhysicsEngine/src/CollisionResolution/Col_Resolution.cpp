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
	Took = 0;
	VelTook = 0;
	if (Cnt.size() != 0)
		if (Cnt[0]->Bods[0]->GetParticle())
		{
			glm::vec3 Vec0 = Cnt[0]->Bods[0]->GetParticle()->GetVel();
			glm::vec3 Pos0 = Cnt[0]->Bods[0]->GetPos() + Vec0 * Cnt[0]->dt0;
			glm::vec3 Pos1 = Cnt[0]->Bods[1]->GetPos();
			glm::vec3 C = (Cnt[0]->R1[0] + Pos1) - (Cnt[0]->R0[0] + Pos0);
			float CN =  glm::dot(C, Cnt[0]->Normal);
			float B = 0.3f;
			float R = Cnt[0]->Restituion * glm::dot( -Vec0, Cnt[0]->Normal);
			Vec0.z = 0.f;//R + (B / Cnt[0]->dt1) * CN;
			Cnt[0]->Bods[0]->GetParticle()->SetVel(Vec0);
			Pos0 += Cnt[0]->dt1 * Vec0;
			Cnt[0]->Bods[0]->SetPosition(Pos0);
		}
}

/////////////////////////////////////////////////////////////////////////
//----------------------------Contact class----------------------------//
/////////////////////////////////////////////////////////////////////////