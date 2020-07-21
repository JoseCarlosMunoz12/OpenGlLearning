#include "Particle.h"
using namespace CoatlPhysicsEngine;

void Particle::UpdateVel(float dt)
{
	if (glm::abs(this->Acum_Force.x) < 0.0625f)
		this->Acum_Force.x = 0.f;
	if (glm::abs(this->Acum_Force.y) < 0.0625f)
		this->Acum_Force.y = 0.f;
	if (glm::abs(this->Acum_Force.z) < 0.625f)
		this->Acum_Force.z = 0.f;
	glm::vec3 Acc = this->Acum_Force * InverMass;
	this->Vel = Acc * dt + Vel;
	Vel = Vel* glm::pow(Damping, dt);
}

Particle::Particle(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp)
	: Acum_Force(glm::vec3(0.f)),PhysicsID(0)
{
	this->AxisAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->Pos = Pos;
	this->InverMass = 0.f;
	this->Vel = InitVel;
	this->Damping = InitDamp;
}

Particle::Particle(glm::vec3 Pos, glm::vec3 InitVel)
	:Damping(5.f), Acum_Force(glm::vec3(0.f)), PhysicsID(0)
{
	this->AxisAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->Pos = Pos;
	this->Vel = InitVel;
	this->InverMass = 0.f;
}

Particle::Particle(glm::vec3 Pos)
	:Vel(glm::vec3(0.f)), InverMass(10.f),Damping(5.f), Acum_Force(glm::vec3(0.f)), PhysicsID(0)
{
	this->AxisAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->Pos = Pos;
}

Particle::~Particle()
{
}

glm::vec3 Particle::UpdatePos(float dt)
{
	//Eulor Method
	this->UpdateVel(dt);
	glm::vec3 NewPos = Pos + Vel * dt;
	this->Pos = NewPos;
	return NewPos;
}

void Particle::SetVel(glm::vec3 NewVel)
{
	this->Vel = NewVel;
}

glm::vec3 Particle::GetVel()
{
	return this->Vel;
}

glm::quat Particle::GetQuat()
{
	return this->AxisAngle;
}

glm::vec3 Particle::GetPos()
{
	return this->Pos;
}

float Particle::GetMass()
{
	return 1 / this->InverMass;
}

void Particle::ChangeInverseMass(float Mass)
{
	this->InverMass = 1 / Mass;
}

void Particle::ResetForce()
{
	this->Acum_Force = glm::vec3(0.f);
}

void Particle::AcumForce(glm::vec3 Force)
{
	this->Acum_Force += Force;
}

void Particle::SetMass(float Mass)
{
	this->InverMass = 1 / Mass;
}

void Particle::SetPos(glm::vec3 Pos)
{
	this->Pos = Pos;
}

void Particle::SetQuat(glm::quat NewQuat)
{
	this->AxisAngle = NewQuat;
}
