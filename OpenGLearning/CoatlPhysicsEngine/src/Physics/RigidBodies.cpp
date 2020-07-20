#include "RigidBodies.h"
using namespace CoatlPhysicsEngine;

RigidBodies::RigidBodies(glm::vec3 Pos)
	:Particle(Pos)
{
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel)
	:Particle(Pos, InitVel)
{
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp)
	:Particle(Pos, InitVel,InitDamp)
{
}

RigidBodies::~RigidBodies()
{
}
