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
			glm::vec3 Pos = Cnt[0]->ContactPoint[0];
			Vec.z = -(.0003f / dt) * Pos.z;
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
