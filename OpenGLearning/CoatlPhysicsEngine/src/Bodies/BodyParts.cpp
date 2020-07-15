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

void BodyParts::UpdateMatrix()
{

}
