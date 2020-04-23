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
	//Check if Positions are the same, instant collision
	std::vector<glm::vec3> Tr1Seg = Tr1.GetSegments();
	for (auto& ii : Tr1Seg)
	{
		ii = ii + Tr1Pos;
	}
	if (Tr0Pos.x == Tr1Pos.x && Tr0Pos.y == Tr1Pos.y && Tr0Pos.z == Tr1Pos.z)
	{
		return true;
	}
	glm::vec3 Tr0Cross = glm::cross((Tr0Seg[1] - Tr0Seg[0]), (Tr0Seg[2] - Tr0Seg[1]));
	glm::vec3 Tr1Cross = glm::cross((Tr1Seg[1] - Tr1Seg[0]), (Tr1Seg[2] - Tr1Seg[1]));
	glm::vec3 Cross = glm::cross(Tr0Cross, Tr1Cross);
	float Val = glm::dot(Cross, Cross);
	if (Val == 0)
	{
		return false;
	}
	//if not parallel check with SAT algorithm on all sides
	for (int ii = 0; ii < 3; ii++)
	{
		float Va = ii + 1;
		if (ii == 2)
		{
			Va = 0;
		}
		if (MATH::ProjColl({Tr0Seg[ii],Tr0Seg[Va]}, Tr0Seg, Tr1Seg))
		{
			return true;
		}
	}

	for (int ii = 0; ii < 3; ii++)
	{
		float Va = ii + 1;
		if (ii == 2)
		{
			Va = 0;
		}
		if (MATH::ProjColl({ Tr1Seg[ii],Tr1Seg[Va] }, Tr0Seg, Tr1Seg))
		{
			return true;
		}
	}
	return false;
}

TriangleColTriangle::TriangleColTriangle()
{
}

TriangleColTriangle::~TriangleColTriangle()
{
}
