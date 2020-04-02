#include "Capsule.h"
using namespace CoatlPhysicsEngine;

Capsule::Capsule(glm::vec3 Pos, float InitRadius, float InitLength)
	:ColShapes(Pos)
{
	this->Radius = InitRadius;
	this->BPos = Pos + glm::vec3(0.f,0.f,InitLength/2);
	this->APos = Pos - glm::vec3(0.f, 0.f, InitLength / 2);
}
Capsule::~Capsule()
{
}

float Capsule::GetRadius()
{
	return this->Radius;
}

glm::vec3 Capsule::ClosestPoint(glm::vec3 Point)
{
	glm::vec3 AB = this->BPos - this->APos;
	float s = glm::dot(AB, AB);
	float d = glm::dot((Point - this->APos), AB) / s;
	if (d < 0.f)
		d = 0.f;
	if (d > 1.f)
		d = 1.f;

	glm::vec3 sed = this->APos + d * AB;

	std::cout << "df";

	return glm::vec3();
}
