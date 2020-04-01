#include "Capsule.h"
using namespace CoatlPhysicsEngine;

Capsule::Capsule(glm::vec3 Pos, float InitRadius, float InitLength)
	:ColShapes(Pos)
{
	this->Radius = InitRadius;
	this->Length = InitLength;
}
Capsule::~Capsule()
{
}

float Capsule::GetRadius()
{
	return this->Radius;
}

float Capsule::GetLength()
{
	return this->Length;
}
