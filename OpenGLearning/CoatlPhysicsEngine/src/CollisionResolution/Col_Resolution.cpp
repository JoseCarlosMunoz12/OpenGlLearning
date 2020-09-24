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
	{
		if (Cnt[0]->Bods[0]->GetParticle())
		{
			glm::vec3 Vec = Cnt[0]->Bods[0]->GetParticle()->GetVel();
			glm::vec3 Pos = Cnt[0]->Bods[0]->GetPos();
			glm::vec3 Dif = (Cnt[0]->R1[0] - Cnt[1]->R0[0]);
			float CN =  glm::dot(Dif, Cnt[0]->Normal);
			Vec.z = (.3f / dt) * CN;
			Cnt[0]->Bods[0]->GetParticle()->SetVel(Vec);
			Cnt[0]->Bods[0]->GetParticle()->SetPos(Pos + Vec * dt);
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//----------------------------Contact class----------------------------//
/////////////////////////////////////////////////////////////////////////

Contacts::Contacts()
{
}

Contacts::~Contacts()
{
}
