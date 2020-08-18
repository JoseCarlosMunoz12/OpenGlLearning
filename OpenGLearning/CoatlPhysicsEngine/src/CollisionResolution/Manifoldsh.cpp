#include "Manifoldsh.h"

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
