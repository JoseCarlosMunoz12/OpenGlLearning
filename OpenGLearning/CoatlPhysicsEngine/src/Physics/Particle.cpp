#include "Particle.h"
using namespace CoatlPhysicsEngine;

void Particle::UpdateVel(float dt)
{
	if (glm::abs(this->AccumForce.x) < 0.0625f)
		this->AccumForce.x = 0.f;
	if (glm::abs(this->AccumForce.y) < 0.0625f)
		this->AccumForce.y = 0.f;
	if (glm::abs(this->AccumForce.z) < 0.625f)
		this->AccumForce.z = 0.f;
	glm::vec3 Acc = this->AccumForce * InvMass;
	this->Vel = Acc * dt + Vel;
	Vel = Vel * glm::pow(LinDamp, dt);
}

Particle::Particle(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp)
	:Bod_Base(Pos, InitVel, InitDamp)
{
	this->Pos = Pos;
	this->Vel = InitVel;
}

Particle::Particle(glm::vec3 Pos, glm::vec3 InitVel)
	:Bod_Base(Pos,InitVel)
{
	this->Pos = Pos;
	this->Vel = InitVel;
}

Particle::Particle(glm::vec3 Pos)
	:Bod_Base(Pos)
{
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

float Particle::GetMass()
{
	return 1 / this->InvMass;
}

void Particle::ChangeInverseMass(float Mass)
{
	this->InvMass = 1 / Mass;
}

void Particle::ResetForce()
{
	this->AccumForce = glm::vec3(0.f);
}

void Particle::AcumForce(glm::vec3 Force)
{
	this->AccumForce += Force;
}

void Particle::SetMass(float Mass)
{
	this->InvMass = 1 / Mass;
}
