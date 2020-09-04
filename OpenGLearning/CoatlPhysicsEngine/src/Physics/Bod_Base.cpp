#include "Bod_Base.h"
using namespace CoatlPhysicsEngine;

Bod_Base::Bod_Base(glm::vec3 InitPos)
	:Pos(InitPos),AccumForce(glm::vec3(0.f)),Vel(glm::vec3(0.f)), LinDamp(01.f), InvMass(0.f)
	, RotVel(glm::vec3(0.f)), RotDamp(1.f),Motion(0.f),Accel(glm::vec3(0.f,0.f,0.f))
	,PrevAccel(glm::vec3(0.f)), Friction(10.f), Restitution(0.4f)
{
	this->AxisAngle = glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f));
}

Bod_Base::Bod_Base(glm::vec3 InitPos, glm::vec3 InitVel)
	: Pos(InitPos), AccumForce(glm::vec3(0.f)), Vel(InitVel) , Accel(glm::vec3(0.f, 0.f, 0.f)),
	LinDamp(.8f), InvMass(0.f),RotVel(glm::vec3(01.f)),RotDamp(1.f)	
	, Motion(0.f), PrevAccel(glm::vec3(0.f)),Friction(10.f), Restitution(0.4f)
{
	this->AxisAngle = glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f));
}

Bod_Base::Bod_Base(glm::vec3 InitPos, glm::vec3 InitVel, float InitDamp)
	: Pos(InitPos), AccumForce(glm::vec3(0.f)), Vel(InitVel), Accel(glm::vec3(0.f, 0.f, 0.f)),
	LinDamp(InitDamp), InvMass(0.f), RotVel(glm::vec3(.8f)), RotDamp(1.f)
	, Motion(0.f), PrevAccel(glm::vec3(0.f)), Friction(10.f), Restitution(0.4f)
{
	this->AxisAngle = glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f));
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

void Bod_Base::AddVel(glm::vec3 DelVel)
{
	this->Vel += DelVel;
}

void Bod_Base::SetId(int NewId)
{
	this->PhysicsID = NewId;
}

void Bod_Base::SetPos(glm::vec3 NewPos)
{
	this->Pos = NewPos;
}


glm::vec3 Bod_Base::GetPos()
{
	return this->Pos;
}

glm::vec3 Bod_Base::GetVel()
{
	return this->Vel;
}
