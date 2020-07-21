#include "RigidBodies.h"
using namespace CoatlPhysicsEngine;

void RigidBodies::UpdateRot(float dt)
{

}

void RigidBodies::UpdateMatrix()
{
	this->TransformMatrix = glm::mat4(1.f);
	glm::mat4 Rot = glm::mat4_cast(this->AxisAngle);
	this->TransformMatrix = glm::translate(this->TransformMatrix, this->Pos) * Rot;
}

void RigidBodies::CalcDerivedData()
{
	this->AxisAngle = glm::normalize(this->AxisAngle);
	this->UpdateMatrix();

}

RigidBodies::RigidBodies(glm::vec3 Pos)
	:Particle(Pos), TransformMatrix(glm::mat4(1.f)),
	RotDamp(1),RotVel(glm::vec3(0.f)),TorqueAcum(glm::vec3(0.f))
{
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel)
	:Particle(Pos, InitVel), TransformMatrix(glm::mat4(1.f)),
	RotDamp(1), RotVel(glm::vec3(0.f)), TorqueAcum(glm::vec3(0.f))
{
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp)
	:Particle(Pos, InitVel,InitDamp), TransformMatrix(glm::mat4(1.f)),
	RotDamp(1), RotVel(glm::vec3(0.f)), TorqueAcum(glm::vec3(0.f))
{
}

void RigidBodies::SetInertia(glm::mat3 InitInitertia)
{
	this->InvIntertia = glm::inverse(InitInitertia);
}

glm::vec3 RigidBodies::UpdatePos(float dt)
{
	this->UpdateVel(dt);
	glm::vec3 NewPos = Pos + Vel * dt;
	this->Pos = NewPos;

	return NewPos;
}

RigidBodies::~RigidBodies()
{
}
