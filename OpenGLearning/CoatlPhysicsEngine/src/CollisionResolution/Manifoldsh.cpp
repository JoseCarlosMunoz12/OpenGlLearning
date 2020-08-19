#include "Manifoldsh.h"

glm::mat3 Contact::MakeSkew(glm::vec3 Vec)
{
	glm::vec3 Col0(   0.f,  Vec.z,-Vec.y);
	glm::vec3 Col1(-Vec.z,    0.f, Vec.x);
	glm::vec3 Col2( Vec.y, -Vec.x,   0.f);
	return glm::mat3(Col0, Col1,Col2);
}

void Contact::CalculateContactbasis()
{
	glm::vec3 ContactTanget[2];
	if (glm::abs(Normal.x) > glm::abs(Normal.y))
	{
		float S = 1.f / glm::sqrt(Normal.z * Normal.z + Normal.x * Normal.x);
		//new X-Axis is at right angles ot eh world X-Axis
		ContactTanget[0].x = Normal.z * S;
		ContactTanget[0].y = 0.f;
		ContactTanget[0].z = -Normal.x * S;
		//the New Y Axis ist at right angles to the new x- Z- axis
		ContactTanget[1].x = Normal.y * ContactTanget[0].x;
		ContactTanget[1].y = Normal.z * ContactTanget[0].x - Normal.x * ContactTanget[0].z;
		ContactTanget[1].z = -Normal.y * ContactTanget[0].x;
	}
	else
	{
		float S = 1.f / glm::sqrt(Normal.z * Normal.z + Normal.y * Normal.y);
		//new X-Axis is at right angles ot eh world X-Axis
		ContactTanget[0].x = 0.f;
		ContactTanget[0].y = -Normal.z * S;
		ContactTanget[0].z = Normal.y * S;
		//the New Y Axis ist at right angles to the new x- Z- axis
		ContactTanget[1].x = Normal.y * ContactTanget[0].z - Normal.z * ContactTanget[0].y;
		ContactTanget[1].y = -Normal.x * ContactTanget[0].z;
		ContactTanget[1].z = Normal.x * ContactTanget[0].y;

	}
	this->ContactToWorld = glm::mat3(Normal, ContactTanget[0], ContactTanget[1]);
}

glm::vec3 Contact::CalculateLocalvel(std::shared_ptr<Bodies> Bod, int ID, float dt)
{
	glm::vec3 ContactVel = glm::vec3(0.f);
	if (!Bod->GetParticle())
		return ContactVel;
	glm::vec3 Vel = glm::cross(Bod->GetParticle()->GetRotVel(), RelContact[ID]);
	Vel += Bod->GetParticle()->GetVel();
	ContactVel = this->ContactToWorld * Vel;
	glm::vec3 AccVel = Bod->GetParticle()->GetAccel() * dt;
	AccVel.x = 0.f;
	ContactVel += AccVel;
	return ContactVel;
}

void Contact::CalculateDesVel(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1, float dt)
{
	const static float VelLimit = 0.25f;
	float VelFromAcc = 0.f;

	if (Bod0->GetParticle()->GetAwakeStatus())
		VelFromAcc += dt *(glm::dot( Bod0->GetParticle()->GetAccel(), Normal));
	if(Bod1->GetParticle() && Bod1->GetParticle()->GetAwakeStatus())
		VelFromAcc -= dt * (glm::dot(Bod1->GetParticle()->GetAccel(), Normal));
	float ThisRest = Restituion;
	if (glm::abs(ContactVelocity.x) < ThisRest)
		ThisRest = 0.f;
	this->DesDeltaVel - ContactVelocity.x - ThisRest * (ContactVelocity.x - VelFromAcc);
}

glm::vec3 Contact::CalcFricImpulse(std::shared_ptr<Bodies> Bods[2], glm::mat3 InvInTn[2])
{
	glm::vec3 ImpContact;
	float InvMass = 1.f / Bods[0]->GetParticle()->GetMass();

	return glm::vec3();
}

glm::vec3 Contact::CalcNonFricImpulse(std::shared_ptr<Bodies> Bod0[2], glm::mat3 InvInTn[2])
{
	return glm::vec3();
}

Contact::Contact()
	:ContactPoint(glm::vec3(0.f)), Normal(glm::vec3(0.f, 0.f, 1.f)),
	Penetration(0.f), Friction(1.f),Restituion(1.f)
{
}
	
Contact::~Contact()
{
}

void Contact::CalculateInternals(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1, float dt)
{
	this->CalculateContactbasis();
	this->RelContact[0] = ContactPoint - Bod0->GetPos();
	this->RelContact[1] = ContactPoint - Bod1->GetPos();
	this->ContactVelocity = this->CalculateLocalvel(Bod0, 0,dt);
	this->ContactVelocity -= this->CalculateLocalvel(Bod1, 1,dt);
	this->CalculateDesVel(Bod0, Bod1, dt);
}

void Contact::ApplyPositionChange(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1,
	glm::vec3 LinChang[2], glm::vec3 AngChange[2])
{
	std::shared_ptr<Bodies> Bods[2] = { Bod0, Bod1 };
	const float AngLimit = .2f;
	float AngM[2];
	float LinM[2];

	float TotalInertia = 0.f;
	float LinInertia[2];
	float Anginertia[2];
	//Calculate inertia of each object in the direction of the contact normal
	for (int ii = 0; ii < 2; ii++) if (Bods[ii]->GetParticle())
	{
		glm::mat3 invInert = Bods[ii]->GetParticle()->GetInertiaWorld();
		invInert = glm::inverse(invInert);
		glm::vec3 AngInertW = glm::cross(RelContact[ii], Normal);
		AngInertW = invInert * AngInertW;
		AngInertW = glm::cross(AngInertW, RelContact[ii]);
		Anginertia[ii] = glm::dot(AngInertW, Normal);
		LinInertia[ii] = 1 / Bods[ii]->GetParticle()->GetMass();
		TotalInertia += LinInertia[ii] + Anginertia[ii];
	}
	//loop through again calculating and applying the changes
	for (int ii = 0; ii < 2; ii++)if (Bods[ii]->GetParticle())
	{
		float Sign = (ii == 0) ? 1 : -1;
		AngM[ii] = Sign * Penetration * (Anginertia[ii] / TotalInertia);
		LinM[ii] = Sign * Penetration * (LinInertia[ii] / TotalInertia);

		glm::vec3 Proj = RelContact[ii] - Normal * glm::dot(RelContact[ii],Normal);
		float MaxMag = AngLimit * glm::length(Proj);

		if (AngM[ii] < -MaxMag)
		{
			float TtlMv = AngM[ii] + LinM[ii];
			AngM[ii] = -MaxMag;
			LinM[ii] = TtlMv - AngM[ii];
		}
		else if (AngM[ii] > MaxMag)
		{
			float TtlMv = AngM[ii] + LinM[ii];
			AngM[ii] = MaxMag;
			LinM[ii] = TtlMv - AngM[ii];
		}

		if (AngM[ii] == 0)
			AngChange[ii] = glm::vec3(0.f);
		else
		{
			glm::vec3 TargetAngDir = glm::cross(RelContact[ii], Normal);
			glm::mat3 invIn = Bods[ii]->GetParticle()->GetInertiaWorld();
			invIn = glm::inverse(invIn);
			AngChange[ii] = invIn * TargetAngDir * (AngM[ii] / Anginertia[ii]);
		}

		LinChang[ii] = Normal * LinM[ii];

		glm::vec3 Pos = Bods[ii]->GetPos();
		Pos += Normal * LinM[ii];
		Bods[ii]->SetPosition(Pos);

		glm::quat Q = Bods[ii]->GetQuat();
		glm::quat R = glm::quat(0.f, AngChange[ii]);
		R *=Q;
		Q += 0.5f * R;
		Bods[ii]->SetQuat(Q);
		if (!Bods[ii]->GetParticle()->GetAwakeStatus())
			Bods[ii]->GetParticle()->CalcDerivedData();

	}
}

void Contact::ApplyVelocityChange(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1,
	glm::vec3 VelChang[2], glm::vec3 RotChange[2])
{
	glm::mat3 InvInertia[2];
	std::shared_ptr<Bodies> Bods[2] = { Bod0, Bod1 };
	//Get Inertia Tensor
	InvInertia[0] = glm::inverse(Bods[0]->GetParticle()->GetInertiaWorld());
	if(Bods[0]->GetParticle())
		InvInertia[1] = glm::inverse(Bods[1]->GetParticle()->GetInertiaWorld());
	//Calc Impulse
	glm::vec3 ImpulseCont;
	if (Friction != 0)
		ImpulseCont = this->CalcFricImpulse(Bods, InvInertia);
	else
		ImpulseCont = this->CalcNonFricImpulse(Bods, InvInertia);
	//Convert impule to World Coordinates
	glm::vec3 Impulse = this->ContactToWorld * ImpulseCont;
	//Split impulse to linear and roation parts
	glm::vec3 ImTrque = glm::cross(RelContact[0], Impulse);
	RotChange[0] = InvInertia[0] * ImTrque;
	VelChang[0] = glm::vec3(0.f);
	VelChang[0] = Impulse * 1.f / Bods[0]->GetParticle()->GetMass();

	//Apply Changes
	Bods[0]->GetParticle()->AddVel(VelChang[0]);
	Bods[0]->GetParticle()->AddRotVel(RotChange[0]);

	//Body 2 changes
	if (Bods[1]->GetParticle())
	{
		//Split impulse to linear and roation parts
		glm::vec3 ImTrque = glm::cross(RelContact[1], Impulse);
		RotChange[1] = InvInertia[1] * ImTrque;
		VelChang[1] = glm::vec3(0.f);
		VelChang[1] = Impulse * -1.f / Bods[1]->GetParticle()->GetMass();
		//Apply Changes
		Bods[1]->GetParticle()->AddVel(VelChang[1]);
		Bods[1]->GetParticle()->AddRotVel(RotChange[1]);
	}
}
