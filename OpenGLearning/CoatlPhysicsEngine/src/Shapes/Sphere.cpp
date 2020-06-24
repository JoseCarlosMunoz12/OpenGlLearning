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
	return this->Radius;
}

void Sphere::SetRadius(float NewR)
{
	this->Radius = NewR;
}

void Sphere::SetQuat(glm::quat NewQuat)
{
	this->QuatAngle = NewQuat;
}

glm::vec3 Sphere::Support(glm::vec3 Dir)
{
	glm::vec3 Norm = glm::normalize(Dir);
	return this->Pos + (this->Radius + 0.001f) * Norm;
}

glm::vec3 Sphere::EPA_Support(glm::vec3 Dir)
{
	return this->Pos;
}

std::vector<glm::vec3> CoatlPhysicsEngine::Sphere::GetVertices()
{
	std::vector<glm::vec3> Temp;
	Temp.push_back(this->Pos);
	return Temp;
}
	