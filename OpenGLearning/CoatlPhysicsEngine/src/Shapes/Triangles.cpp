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
	glm::mat4 R = glm::mat4_cast(this->QuatAngle);
	glm::mat4 T = glm::translate(glm::mat4(1.f), this->Pos);
	std::vector<glm::vec3> SegPoints;
	for (auto& jj : Points)
	{
		glm::vec4 Set = glm::vec4(jj,1.f);
		SegPoints.push_back(T * R * Set);
	}
	return SegPoints;
}

glm::vec3 Triangles::GetClosestPoint(glm::vec3 Point)
{	
	return MATH::ClosestPoint_Seg(GetSegments(), Pos, Point);
}

void Triangles::SetQuat(glm::quat NewQuat)
{
	this->QuatAngle = NewQuat;
}

glm::vec3 Triangles::Support(glm::vec3 Dir)
{
	std::vector<glm::vec3> Pnts = this->GetSegments();
	float S = glm::dot(Pnts[0], Dir);
	glm::vec3 MaxPnt = Pnts[0];
	int Size = Pnts.size();
	for (int ii = 1; ii < Size; ii++)
	{
		float T = glm::dot(Pnts[ii], Dir);
		if (T > S)
		{
			S = T;
			MaxPnt = Pnts[ii];
		}
	}
	return MaxPnt;
}

glm::vec3 Triangles::EPA_Support(glm::vec3 Dir)
{
	return this->Support(Dir);
}

std::vector<glm::vec3> Triangles::GetVertices()
{
	return this->GetSegments();
}

std::vector<glm::vec3> Triangles::GetNormals()
{
	std::vector<glm::vec3> T = this->GetSegments();
	std::vector<glm::vec3> T_N = { glm::normalize(T[1] - T[0]),
		glm::normalize(T[2] - T[0]) };
	return T_N;
}