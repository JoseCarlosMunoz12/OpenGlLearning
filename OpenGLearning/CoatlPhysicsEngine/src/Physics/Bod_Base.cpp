#include "Bod_Base.h"
using namespace CoatlPhysicsEngine;

Bod_Base::Bod_Base(glm::vec3 InitPos)
	:Pos(InitPos),AccumForce(glm::vec3(0.f)),Vel(glm::vec3(0.f)), LinDamp(1.f), InvMass(0.f)
{
}

Bod_Base::Bod_Base(glm::vec3 InitPos, glm::vec3 InitVel)
	: Pos(InitPos), AccumForce(glm::vec3(0.f)), Vel(InitVel),
	LinDamp(1.f), InvMass(0.f)
{
}

Bod_Base::Bod_Base(glm::vec3 InitPos, glm::vec3 InitVel, float InitDamp)
	: Pos(InitPos), AccumForce(glm::vec3(0.f)), Vel(InitVel),
	LinDamp(InitDamp), InvMass(0.f)
{
}

Bod_Base::~Bod_Base()
{
}

void Bod_Base::SetMass(float Mass)
{
	if (Mass != 0.f)
		this->InvMass = 1 / Mass;
	else
		this->InvMass = 0.f;
}

void Bod_Base::SetVel(glm::vec3 SetVel)
{
	this->Vel = SetVel;
}

void Bod_Base::SetId(int NewId)
{
	this->PhysicsID = NewId;
}
