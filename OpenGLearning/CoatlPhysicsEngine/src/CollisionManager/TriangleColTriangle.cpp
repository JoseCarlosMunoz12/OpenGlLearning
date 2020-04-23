#include "TriangleColTriangle.h"

bool TriangleColTriangle::TrColTr(Triangles Tr0, Triangles Tr1)
{
	//Check Normal vectors  if they are parrallel
	std::vector<glm::vec3> Tr0Seg = Tr0.GetSegments();
	std::vector<glm::vec3> Tr1Seg = Tr1.GetSegments();
	glm::vec3 Tr0Cross = glm::cross((Tr0Seg[1] - Tr0Seg[0]), (Tr0Seg[2] - Tr0Seg[1]));
	glm::vec3 Tr1Cross = glm::cross((Tr1Seg[1] - Tr1Seg[0]), (Tr1Seg[2] - Tr1Seg[1]));
	glm::vec3 Cross = glm::cross(Tr0Cross, Tr1Cross);
	float Val = glm::dot(Cross, Cross);
	if (Val == 0)
	{
		return false;
	}
	//if not parallel check with SAT algorithm
	//for (int ii = 0; ii < 2; ii++)
	//{
	//	if (MATH::ProjColl({Tr0Seg[ii],Tr0Seg[ii + 1]}, Tr0Seg, Tr1Seg))
	//	{
	//		return false;
	//	}
	//}
	return true;
}

TriangleColTriangle::TriangleColTriangle()
{
}

TriangleColTriangle::~TriangleColTriangle()
{
}
