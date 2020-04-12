#include "Sphere.h"
using namespace CoatlPhysicsEngine;
Sphere::Sphere(glm::vec3 SetPos,float SetRadius)
	:ColShapes(SetPos)
{
	this->Radius = SetRadius;
}

Sphere::~Sphere()
{
}

float Sphere::GetRadius()
{
	glm::vec3 se = MATH::Set();
	return this->Radius;
}

void Sphere::SetRadius(float NewR)
{
	this->Radius = NewR;
}
