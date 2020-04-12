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
	return MATH::ClosestPoint_Seg(GetSegments(), Pos, Point);
}
