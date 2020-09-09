#include "BodyParts.h"
using namespace CoatlPhysicsEngine;

BodyParts::BodyParts()
{
}
BodyParts::BodyParts(std::shared_ptr<ColShapes> InitBodPart)
{
	this->BodPart = InitBodPart;
}

BodyParts::~BodyParts()
{
}

void BodyParts::AddShape(std::shared_ptr<ColShapes> InitBodPart)
{
	this->BodPart = InitBodPart;
}

void BodyParts::AddParticle(std::shared_ptr<Bod_Base> InitParticle)
{
	this->BodParticle = InitParticle;
}

glm::vec3 BodyParts::GetPos()
{
	return this->BodPart->GetPos();
}

glm::quat BodyParts::GetQuatAngle()
{
	return this->BodPart->GetQuatAngle();
}

void BodyParts::SetPos(glm::vec3 NewPos)
{
	this->BodPart->SetPos(NewPos);
	if (this->BodParticle)
		this->BodParticle->SetPos(NewPos);
}

void BodyParts::SetQuat(glm::quat Q)
{
	if (isnan(glm::length(Q)))
		return;
	this->BodPart->SetQuat(Q);
	if (this->BodParticle)
		this->BodParticle->SetQuat(Q);
}

std::shared_ptr<ColShapes> BodyParts::GetShape()
{
	return this->BodPart;
}

std::shared_ptr<Bod_Base> BodyParts::GetParticle()
{
	return this->BodParticle;
}

glm::vec3 BodyParts::Support(glm::vec3 Dir)
{
	return this->BodPart->Support(Dir);
}
