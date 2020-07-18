#include "BodyParts.h"
using namespace CoatlPhysicsEngine;

BodyParts::BodyParts()
{

}
BodyParts::BodyParts(std::shared_ptr<BodyParts> InitParent)
{
	this->Parent = InitParent;
}
BodyParts::BodyParts(std::shared_ptr<BodyParts> InitParent, std::shared_ptr<ColShapes> InitBodPart)
{
	this->Parent = InitParent;
	this->BodPart = InitBodPart;
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
	return this->BodPart->GetMatrix();
}

void BodyParts::UpdateMatrix()
{
	glm::mat4 P = glm::mat4(1.f);
	if (this->Parent)
		P = this->Parent->GetParMatrix() * this->Parent->GetMatrix();
	this->BodPart->SetMatrix(P);
}

glm::vec3 BodyParts::GetPos()
{
	return this->BodPart->GetPos();
}

glm::vec3 BodyParts::GeOffset()
{
	return this->GetPos();
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

std::shared_ptr<Particle> BodyParts::GetParticle()
{
	return this->BodParticle;
}

glm::vec3 BodyParts::GetW_Pos()
{
	return this->GetParMatrix() * glm::vec4(this->GetPos(), 1.f);
}

void BodyParts::SetWolrd_Mod(glm::vec3 W_Pos)
{ 
	this->SetPos(glm::inverse(this->GetParMatrix())* glm::vec4(W_Pos,1.f));
}

glm::vec3 CoatlPhysicsEngine::BodyParts::Support(glm::vec3 Dir)
{
	return this->BodPart->Support(Dir);
}
