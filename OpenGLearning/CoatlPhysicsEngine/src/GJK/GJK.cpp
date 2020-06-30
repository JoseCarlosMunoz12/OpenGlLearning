#include "GJK.h"
using namespace CoatlPhysicsEngine;

float GJK_Alg::F3Box(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	glm::vec3 N = glm::cross(A, B);
	return glm::dot(N, C);
}

glm::vec3 GJK_Alg::TripleCross(glm::vec3 A, glm::vec3 B)
{
	glm::vec3 T = glm::cross(A, B);
	glm::vec3 Result = glm::cross(T, A);	
	return MATH::Normalize(Result);
}

glm::vec3 GJK_Alg::Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir)
{
	return Shape0->Support(Dir) - Shape1->Support(-Dir);
}

bool GJK_Alg::Simplex_Maker(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
	std::vector<glm::vec3> Verts, glm::vec3& Dir)
{
	int Size = Verts.size();
	switch (Size)
	{
	case 0: {
		Dir = MATH::Normalize(Shape0->GetPos() - Shape1->GetPos());
	}break;
	case 1: {
		Dir = -MATH::Normalize(Verts[0]);
	}break;
	case 2: {
		glm::vec3 AB = MATH::Normalize(Verts[0] - Verts[1]);
		glm::vec3 A0 = -MATH::Normalize(Verts[1]);
		if (glm::dot(AB,A0) > 0.f)
			Dir = TripleCross(AB, A0);
		else
			Dir = A0;
	}break;
	case 3: {
		glm::vec3 A = Verts[2];
		glm::vec3 B = Verts[1];
		glm::vec3 C = Verts[0];

		glm::vec3 AB = B - A;
		glm::vec3 AC = C - A;

		glm::vec3 ABC = glm::cross(AB, AC);

		glm::vec3 ABC_AC = glm::cross(ABC, AC);
		glm::vec3 AB_ABC = glm::cross(AB, ABC);

		if (glm::dot(ABC_AC, AC) > 0.f)
		{

		}
	}break;
	case 4: {
	}break;
	default:
		break;
	}
	return false;
}

bool GJK_Alg::GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	std::vector<glm::vec3> Verts;
	glm::vec3 Dir;
	Simplex_Maker(Shape0, Shape1,Verts, Dir);
	Verts.push_back(Support(Shape0, Shape1, Dir));
	Simplex_Maker(Shape0, Shape1, Verts, Dir);
	for (int ii = 0; ii < 64; ii++)
	{
		glm::vec3 A = Support(Shape0, Shape1, Dir);
		if (glm::dot(A, Dir) < 0.f)
			return false;
		Verts.push_back(A);
		if (Simplex_Maker(Shape0, Shape1, Verts, Dir))
			return true;
	}
	return false;
}

bool GJK_Alg::EPA_GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3& DistVec)
{
	return false;
}

GJK_Alg::GJK_Alg::GJK_Alg()
{
}

GJK_Alg::GJK_Alg::~GJK_Alg()
{
}
