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
	}break;
	case 3: {
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
		Simplex_Maker(Shape0, Shape1, Verts, Dir);
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
