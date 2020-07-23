#include "RigidBodies.h"
using namespace CoatlPhysicsEngine;

void RigidBodies::UpdateRot(float dt)
{
	this->AngularAccelration += this->InvInertia * this->TorqueAcum;
	this->RotVel = this->AngularAccelration * dt;
	this->RotVel *= glm::pow(this->RotDamp, dt);
	glm::quat E = glm::quat(0, this->RotVel * dt);
	E *= this->GetQuat();
	this->SetQuat(this->GetQuat() + E *.5f);
	
	this->CalcDerivedData();
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

void RigidBodies::TransformInertiaTensor()
{

}

RigidBodies::RigidBodies(glm::vec3 Pos)
	:Particle(Pos), TransformMatrix(glm::mat4(1.f)),
	RotDamp(1),RotVel(glm::vec3(0.f)),TorqueAcum(glm::vec3(0.f)),AngularAccelration(glm::vec3(0.f))
{
	this->SetInertia(glm::mat3(1.f));
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel)
	:Particle(Pos, InitVel), TransformMatrix(glm::mat4(1.f)),
	RotDamp(1), RotVel(glm::vec3(0.f)), TorqueAcum(glm::vec3(0.f)), AngularAccelration(glm::vec3(0.f))
{
	this->SetInertia(glm::mat3(1.f));
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp)
	:Particle(Pos, InitVel,InitDamp), TransformMatrix(glm::mat4(1.f)),
	RotDamp(1), RotVel(glm::vec3(0.f)), TorqueAcum(glm::vec3(0.f)), AngularAccelration(glm::vec3(0.f))
{
	this->SetInertia(glm::mat3(1.f));
}

RigidBodies::~RigidBodies()
{
}

void RigidBodies::SetInertia(glm::mat3 InitInertia)
{
	this->InvInertia = glm::inverse(InitInertia);
}

void RigidBodies::AddTorque(glm::vec3 Torque)
{
	this->TorqueAcum += Torque;	
}

void RigidBodies::SetAwake(bool Awake)
{
	if (Awake)
	{
		this->IsAwake = true;
		this->Motion = 0.001f * 2.f;
	}
	else
	{
		this->IsAwake = false;
		this->Vel = glm::vec3(0.f);
		this->RotVel = glm::vec3(0.f);
	}
}

glm::vec3 RigidBodies::UpdatePos(float dt)
{
	if (!this->IsAwake)
		return this->Pos;
	this->UpdateVel(dt);
	//this->UpdateRot(dt);
	glm::vec3 NewPos = Pos + Vel * dt;
	this->Pos = NewPos;
	if (this->CanSleep)
	{
		float CurMotion = glm::dot(this->Vel, this->Vel);
		CurMotion += glm::dot(this->RotVel, this->RotVel);
		float Bias = glm::pow(0.f, dt);
		this->Motion = Bias * this->Motion + (1 - Bias) * CurMotion;
		if (this->Motion < 0.0001)
			this->SetAwake(false);
		else if (this->Motion > 10 * 0.001)
			Motion = 10 * 0.001;
	}
	return NewPos;
}