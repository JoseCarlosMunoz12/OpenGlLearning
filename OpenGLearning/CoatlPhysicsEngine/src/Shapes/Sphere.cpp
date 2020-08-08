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
	return this->Pos + (this->Radius) * Norm;
}

glm::vec3 Sphere::EPA_Support(glm::vec3 Dir)
{
	return this->Pos;
}

std::vector<glm::vec3> Sphere::GetVertices()
{
	std::vector<glm::vec3> Temp;
	Temp.push_back(this->Pos);
	return Temp;
}

glm::mat3 Sphere::GetInertia(float Mass)
{
	float Mid = 2.f / 5.f * Radius * Radius;
	glm::vec3 Col0 = glm::vec3(Mid, 0.f, 0.f);
	glm::vec3 Col1 = glm::vec3(0.f, Mid, 0.f);
	glm::vec3 Col2 = glm::vec3(0.f, 0.f, Mid);
	return Mass * glm::mat3(Col0, Col1, Col2);
}
	