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

float CoatlPhysicsEngine::Sphere::GetRadius()
{
	return this->Radius;
}

std::string Sphere::GetShapeType()
{
	return "Sphere";
}
