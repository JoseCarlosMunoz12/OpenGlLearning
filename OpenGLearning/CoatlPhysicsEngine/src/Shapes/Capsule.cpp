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

glm::vec3 Capsule::ClosestPoint_P(glm::vec3 Point)
{
	glm::vec3 AB = this->BPos - this->APos;
	float s = glm::dot(AB, AB);
	float d = glm::dot((Point - this->APos), AB) / s;
	if (d < 0.f)
		d = 0.f;
	if (d > 1.f)
		d = 1.f;
	return this->APos + d * AB;	
}

float Capsule::Distance(glm::vec3 Point)
{
	glm::vec3 P1 = this->ClosestPoint_P(Point);
	return glm::distance(P1, Point);
}

float Capsule::Distance(std::vector<glm::vec3> Segment)
{
	float EPSILON = 0.00000001;

	return 0.0f;
}

std::vector<glm::vec3> CoatlPhysicsEngine::Capsule::GetSegment()
{
	return {this->APos,this->BPos};
}
