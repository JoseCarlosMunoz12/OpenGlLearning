#include "RigidBodies.h"
using namespace CoatlPhysicsEngine;

void RigidBodies::UpdateRot(float dt)
{
	this->AngularAccelration += this->InvInertia * this->TorqueAcum;
	this->RotVel = this->AngularAccelration * dt;
	this->RotVel *= glm::pow(this->RotDamp, dt);
	glm::quat E = glm::quat(0, this->RotVel * dt);
	E *= this->AxisAngle;
	this->AxisAngle += E *.5f;	
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
	this->TransformInertiaTensor();
}

void RigidBodies::TransformInertiaTensor()
{
	glm::mat4 RotMat = glm::mat4_cast(this->AxisAngle);
	glm::mat4 InvInertia = glm::mat4(this->InvInertia);
	this->InvIntertiaWSpace = RotMat * InvInertia * glm::inverse(RotMat);
}

void RigidBodies::UpdateVel(float dt)
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

RigidBodies::RigidBodies(glm::vec3 Pos)
	:Bod_Base(Pos), TransformMatrix(glm::mat4(1.f)),
	RotDamp(1),RotVel(glm::vec3(0.f)),TorqueAcum(glm::vec3(0.f)),AngularAccelration(glm::vec3(0.f))
{
	this->SetInertia(glm::mat3(1.f));
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel)
	:Bod_Base(Pos, InitVel), TransformMatrix(glm::mat4(1.f)),
	RotDamp(1), RotVel(glm::vec3(0.f)), TorqueAcum(glm::vec3(0.f)), AngularAccelration(glm::vec3(0.f))
{
	this->SetInertia(glm::mat3(1.f));
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp)
	:Bod_Base(Pos, InitVel,InitDamp), TransformMatrix(glm::mat4(1.f)),
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

void RigidBodies::AcumTorque(glm::vec3 Torque)
{
	this->TorqueAcum += Torque;
}

void RigidBodies::SetAwake(bool Awake)
{
	if (Awake)
	{
		this->IsAwake = true;
		this->Motion = Epsilon * 2.f;
	}
	else
	{
		this->IsAwake = false;
		this->Vel = glm::vec3(0.f);
		this->RotVel = glm::vec3(0.f);
	}
}

void RigidBodies::SetQuat(glm::quat NewQuat)
{
	this->AxisAngle = NewQuat;
}

void RigidBodies::AcumForce(glm::vec3 Force)
{
	this->AccumForce += Force;
	this->IsAwake = true;
}

void RigidBodies::ResetForce()
{
	this->AccumForce = glm::vec3(0.f);
	this->TorqueAcum = glm::vec3(0.f);
}

glm::vec3 RigidBodies::UpdatePos(float dt)
{
	if (!this->IsAwake)
		return this->Pos;
	this->UpdateVel(dt);
	this->UpdateRot(dt);
	glm::vec3 NewPos = Pos + Vel * dt;
	this->Pos = NewPos;
	if (this->CanSleep)
	{
		float CurMotion = glm::dot(this->Vel, this->Vel);
		CurMotion += glm::dot(this->RotVel, this->RotVel);
		float Bias = glm::pow(0.f, dt);
		this->Motion = Bias * this->Motion + (1 - Bias) * CurMotion;
		if (this->Motion < Epsilon)
			this->SetAwake(false);
		else if (this->Motion > 10 * Epsilon)
			Motion = 10 * Epsilon;
	}
	return NewPos;
}

bool RigidBodies::GetAwakeStatus()
{
	return this->IsAwake;
}

void RigidBodies::SetCanSleep(bool Can)
{
	this->CanSleep = Can;
	if (!this->CanSleep && !this->IsAwake)
		this->SetAwake(true);
}
