#include "Triangles.h"
using namespace CoatlPhysicsEngine;

Triangles::Triangles(glm::vec3 InitPos, std::vector<glm::vec3> InitPoints)
	:ColShapes(InitPos)
{
	for (int ii = 0; ii < 3; ii++)
	{
		this->Points[ii] = InitPoints[ii];
	}
}

Triangles::~Triangles()
{
}

std::vector<glm::vec3> Triangles::GetSegments()
{
	std::vector<glm::vec3> SegPoints(std::begin(Points), std::end(Points));
	return SegPoints;
}

glm::vec3 CoatlPhysicsEngine::Triangles::GetClosestPoint(glm::vec3 Point)
{
	glm::vec3 AB = Points[1] - Points[0];
	glm::vec3 AC = Points[2] - Points[0];
	glm::vec3 AP = Point - (Points[0] + Pos);
	float D1 = glm::dot(AB, AP);
	float D2 = glm::dot(AC, AP);
	if (D1 <= 0.f && D2 <= 0.f)
		return (Points[0] + Pos);

	glm::vec3 BP = Point - (Points[1] + Pos);
	float D3 = glm::dot(AB, BP);
	float D4 = glm::dot(AC, BP);
	if (D3 >= 0.f && D4 <= D3)
		return (Points[1] + Pos);

	float VC = D1 * D4 - D3 * D2;
	if (VC <= 0.f && D1 >= 0.f && D3 <= 0.f)
	{
		float V = D1 /(D1 - D3);
		return Pos + Points[0] + V * AB;
	}



	return glm::vec3();
}
