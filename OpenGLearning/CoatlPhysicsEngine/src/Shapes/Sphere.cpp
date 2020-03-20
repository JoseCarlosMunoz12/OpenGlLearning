#include "Sphere.h"
using namespace CoatlPhysicsEngine;
Sphere::Sphere(float SetRadius)
{
	this->Radius = SetRadius;
}

Sphere::~Sphere()
{
}

float CoatlPhysicsEngine::Sphere::GetRadius()
{
	return this->Radius;
}

std::string Sphere::GetShapeType()
{
	return "Sphere";
}
