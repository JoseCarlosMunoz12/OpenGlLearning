#include "RigidBodies.h"
using namespace CoatlPhysicsEngine;

RigidBodies::RigidBodies(glm::vec3 Pos)
	:Particle(Pos),
	RotDamp(1),RotVel(glm::vec3(0.f)),TorqueAcum(glm::vec3(0.f))
{
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel)
	:Particle(Pos, InitVel),
	RotDamp(1), RotVel(glm::vec3(0.f)), TorqueAcum(glm::vec3(0.f))
{
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp)
	:Particle(Pos, InitVel,InitDamp),
	RotDamp(1), RotVel(glm::vec3(0.f)), TorqueAcum(glm::vec3(0.f))
{
}

void RigidBodies::SetInertia(glm::mat3 InitInitertia)
{
	this->InvIntertia = glm::inverse(InitInitertia);
}

RigidBodies::~RigidBodies()
{
}
