#include "BodyParts.h"
using namespace CoatlPhysicsEngine;

BodyParts::BodyParts()
{

}

BodyParts::~BodyParts()
{
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
