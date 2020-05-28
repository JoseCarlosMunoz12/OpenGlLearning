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
	glm::mat4 RotMat = glm::mat4_cast(this->QuatAngle);
	std::vector<glm::vec3> SegPoints;
	for (auto& jj : Points)
	{
		jj = jj + this->Pos;
		glm::vec4 Set = glm::vec4(jj.x, jj.y, jj.z, 0.f);
		Set = Set * RotMat;
		SegPoints.push_back(glm::vec3(Set.x, Set.y, Set.z));
	}
	return SegPoints;
}

glm::vec3 CoatlPhysicsEngine::Triangles::GetClosestPoint(glm::vec3 Point)
{	
	return MATH::ClosestPoint_Seg(GetSegments(), Pos, Point);
}

void Triangles::SetQuat(glm::quat NewQuat)
{
	this->QuatAngle = NewQuat;
}

