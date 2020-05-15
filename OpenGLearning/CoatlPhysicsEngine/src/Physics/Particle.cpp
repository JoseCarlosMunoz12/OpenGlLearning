#include "Particle.h"
using namespace CoatlPhysicsEngine;

void Particle::UpdateVel(float dt)
{
	glm::vec3 Acc = this->Acum_Force * InverMass;
	this->Vel = Acc * dt + Vel;
	Vel = Vel* glm::pow(Damping, dt);
}

Particle::Particle(glm::vec3 InitVel, float InitDamp)
	: Acum_Force(glm::vec3(0.f))
{
	this->InverMass = 0.f;
	this->Vel = InitVel;
	this->Damping = InitDamp;
}

Particle::Particle(glm::vec3 InitVel)
	:Damping(1.f), Acum_Force(glm::vec3(0.f))
{
	this->Vel = InitVel;
	this->InverMass = 0.f;
}

Particle::Particle()
	:Vel(glm::vec3(0.f)), InverMass(0.f),Damping(1.f), Acum_Force(glm::vec3(0.f))
{
}

Particle::~Particle()
{
}

glm::vec3 Particle::UpdatePos(glm::vec3 Pos, float dt)
{
	glm::vec3 NewPos = Pos + Vel * dt;
	this->UpdateVel(dt);
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

float Particle::GetInverseMass()
{
	return this->InverMass;
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
