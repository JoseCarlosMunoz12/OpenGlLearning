#include "TriangleColTriangle.h"

bool TriangleColTriangle::TrColTr(Triangles Tr0, Triangles Tr1)
{
	//Check Normal vectors  if they are parrallel
	glm::vec3 Tr0Pos = Tr0.GetPos();
	std::vector<glm::vec3> Tr0Seg = Tr0.GetSegments();
	for (auto& ii : Tr0Seg)
	{
		ii = ii + Tr0Pos;
	}
	glm::vec3 Tr1Pos = Tr1.GetPos();
	std::vector<glm::vec3> Tr1Seg = Tr1.GetSegments();
	for (auto& ii : Tr1Seg)
	{
		ii = ii + Tr1Pos;
	}
	//make And Check planes are on the same side
	C_Plane Tr0_P(Tr0Seg);
	if (Tr0_P.SameLocPl(Tr1Seg))
	{
		return false;
	}
	C_Plane Tr1_P(Tr1Seg);
	if (Tr1_P.SameLocPl(Tr0Seg))
		{
			return false;
		}
	
	//Find Plane Intersection normal and project
	glm::vec3 PlSegDir = glm::cross(Tr0_P.Normal, Tr1_P.Normal);	
	if (MATH::ProjColl({ glm::vec3(0.f),PlSegDir }, Tr0Seg, Tr1Seg))
	{
		return false;
	}
	return true;
}

TriangleColTriangle::TriangleColTriangle()
{
}

TriangleColTriangle::~TriangleColTriangle()
{
}
