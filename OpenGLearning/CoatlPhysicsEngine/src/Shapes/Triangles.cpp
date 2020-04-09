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
	std::vector<glm::vec3> SegPoints;
	for (int ii = 0; ii < 3; ii++)
	{
		for (int jj = ii + 1; jj < 3; jj++)
		{
			SegPoints.push_back(this->Points[ii]);
			SegPoints.push_back(this->Points[jj]);
		}
	}
	return SegPoints;
}
