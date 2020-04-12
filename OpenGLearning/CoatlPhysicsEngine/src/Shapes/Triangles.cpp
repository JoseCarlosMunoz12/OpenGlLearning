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
	/*glm::vec3 AB = Points[1] - Points[0];
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

	glm::vec3 CP = Point - (Points[2] + Pos);
	float D5 = glm::dot(AB, CP);
	float D6 = glm::dot(AC, CP);
	if (D6 >= 0.f && D5 <= D6)
		return (Points[2] + Pos);

	float VB = D5 * D2 - D1 * D6;
	if (VB <= 0.f && D2 >= 0.f && D6 <= 0.f)
	{
		float W = D2 / (D2 - D6);
		return Pos + Points[0] + W * AC;
	}

	float VA = D3 * D6 - D5 * D4;
	if (VA <= 0.f && (D4 - D3) >= 0.f && (D5 - D6) >= 0.f)
	{
		float W = (D4 - D3) / ((D4 - D3) +(D5 -D6));
		return Pos + Points[1] + W * ( Points[2] - Points[1]);
	}
	float Denom = 1.0f / (VA + VB + VC);
	float V = VB * Denom;
	float W = VC * Denom;

	return (Pos + Points[0] + V * AB + W * AC);*/
	return MATH::ClosestPoint_Seg(GetSegments(), Pos, Point);
}
