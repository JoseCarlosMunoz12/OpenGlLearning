#include "RigidBodies.h"
using namespace CoatlPhysicsEngine;

void RigidBodies::UpdateRot(float dt)
{
	glm::vec3 AngularAccelration = this->InvIntertiaWSpace * this->TorqueAcum;
	this->RotVel += AngularAccelration * dt;
	this->RotVel *= glm::pow(this->RotDamp, dt);
	glm::quat E = glm::quat(0, this->RotVel * dt);
	this->AxisAngle =this->AxisAngle * E *.5f + this->AxisAngle;	
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
	glm::mat3 S = glm::inverse(this->InvInertia);
	glm::mat3 InW = glm::mat3_cast(this->AxisAngle);
	this->InvIntertiaWSpace = glm::inverse( InW * S * glm::transpose(InW));
}

void RigidBodies::AddForceAtPoint(glm::vec3 Force, glm::vec3 Pnt)
{
	glm::vec3 Point = Pnt - this->Pos;
	TorqueAcum += glm::cross( Point,Force);
	AccumForce += Force;
	this->IsAwake = true;
}

void RigidBodies::UpdateVel(float dt)
{
	if (glm::abs(this->AccumForce.x) < 0.0625f)
		this->AccumForce.x = 0.f;
	if (glm::abs(this->AccumForce.y) < 0.0625f)
		this->AccumForce.y = 0.f;
	if (glm::abs(this->AccumForce.z) < 0.625f)
		this->AccumForce.z = 0.f;
	glm::vec3 Acc = this->AccumForce * this->InvMass;
	this->PrevAccel = Acc + glm::vec3(0.f,0.f,-9.81f);
	this->Vel = PrevAccel * dt + Vel;
	Vel = Vel * glm::pow(LinDamp, dt);
}

RigidBodies::RigidBodies(glm::vec3 Pos)
	:Bod_Base(Pos), TransformMatrix(glm::mat4(1.f)),
	TorqueAcum(glm::vec3(0.f))
{
	this->SetInertia(glm::mat3(1.f));
	this->TransformInertiaTensor();
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel)
	:Bod_Base(Pos, InitVel), TransformMatrix(glm::mat4(1.f)),
	TorqueAcum(glm::vec3(0.f))
{
	this->SetInertia(glm::mat3(1.f));
	this->TransformInertiaTensor();
}

RigidBodies::RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp)
	:Bod_Base(Pos, InitVel,InitDamp), TransformMatrix(glm::mat4(1.f)),
	TorqueAcum(glm::vec3(0.f))
{
	this->SetInertia(glm::mat3(1.f));
	this->TransformInertiaTensor();
}

RigidBodies::~RigidBodies()
{
}

void RigidBodies::SetInertia(glm::mat3 InitInertia)
{
	this->InvInertia = glm::inverse(InitInertia);
}

glm::mat3 RigidBodies::GetInertiaWorld()
{
	return glm::inverse(this->InvIntertiaWSpace);
}

void RigidBodies::AcumTorque(glm::vec3 Torque)
{
	this->TorqueAcum += Torque;
	this->IsAwake = true;
}

void RigidBodies::AddForceAtBodyPoint(glm::vec3 Force, glm::vec3 Pnt)
{
	//glm::vec3 RelPnt = this->TransformMatrix * glm::vec4(Pnt,1.f);
	this->AddForceAtPoint(Force, Pnt);
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
		this->SetVel(glm::vec3(0.f));
		this->SetRotVel(glm::vec3(0.f));
	}
}

void RigidBodies::SetQuat(glm::quat NewQuat)
{
	this->AxisAngle = glm::normalize(NewQuat);
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

glm::vec3 RigidBodies::GetRotVel()
{
	return this->RotVel;
}

void RigidBodies::SetRotVel(glm::vec3 NewRotVel)
{
	this->RotVel = NewRotVel;
}

void RigidBodies::AddRotVel(glm::vec3 DelRotVel)
{
	this->RotVel += DelRotVel;
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
		float Bias = glm::pow(0.5f, dt);
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

bool RigidBodies::SleepStatus()
{
	return this->CanSleep;
}

void RigidBodies::SetCanSleep(bool Can)
{
	this->CanSleep = Can;
	if (!this->CanSleep && !this->IsAwake)
		this->SetAwake(true);
}
