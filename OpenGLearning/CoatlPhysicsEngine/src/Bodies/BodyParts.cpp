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

void BodyParts::AddParticle(std::shared_ptr<Particle> InitParticle)
{
	this->BodParticle = InitParticle;
}

glm::mat4 BodyParts::GetParMatrix()
{
	return this->BodPart->GetParMatrix();
}

glm::mat4 BodyParts::GetMatrix()
{
	return this->BodPart->ShapeMatrix();;
}

void BodyParts::UpdateMatrix()
{
	glm::mat4 P = glm::mat4(1.f);
	if (this->Parent)
		P = this->Parent->GetParMatrix() * this->Parent->GetMatrix();
	this->BodPart->SetMatrix(P);
}

glm::vec3 CoatlPhysicsEngine::BodyParts::GetPos()
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
	this->BodPart->SetQuat(Q);
	if (this->BodParticle)
		this->BodParticle->SetQuat(Q);
}

std::shared_ptr<ColShapes> BodyParts::GetShape()
{
	return this->BodPart;
}

std::shared_ptr<Particle> CoatlPhysicsEngine::BodyParts::GetParticle()
{
	return this->BodParticle;
}
