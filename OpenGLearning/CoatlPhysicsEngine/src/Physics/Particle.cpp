#include "Particle.h"
using namespace CoatlPhysicsEngine;

void Particle::UpdateVel(glm::vec3 Acc, float dt)
{
	float ValX = glm::pow(this->Vel.x, dt);
	float ValY = glm::pow(this->Vel.y, dt);
	float ValZ = glm::pow(this->Vel.z, dt);
	this->Vel = Acc * dt + Vel;
}

Particle::Particle(glm::vec3 InitVel, float InitDamp)
{
	this->InverMass = 0.f;
	this->Vel = InitVel;
	this->Damping = InitDamp;
}

Particle::Particle(glm::vec3 InitVel)
	:Damping(0.f)
{
	this->Vel = InitVel;
	this->InverMass = 0.f;
}

Particle::Particle()
	:Vel(glm::vec3(0.f)), InverMass(0.f),Damping(1.f)
{
}

Particle::~Particle()
{
}

glm::vec3 Particle::UpdatePos(glm::vec3 Pos, glm::vec3 Ac, float dt)
{
	this->UpdateVel(Ac, dt);
	float TimSqr = dt * dt * .5f;
	glm::vec3 NewPos = Pos + Vel * dt + Ac * TimSqr ;
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
