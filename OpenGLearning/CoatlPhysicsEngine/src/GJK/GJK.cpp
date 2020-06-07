#include "GJK.h"
using namespace CoatlPhysicsEngine;

float GJK_Alg::F3Box(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	glm::vec3 N = glm::cross(A, B);
	return glm::dot(N, C);
}

glm::vec3 GJK_Alg::TripleCross(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	glm::vec3 T = glm::cross(A, B);
	glm::vec3 Result = glm::cross(T, C);
	return Result;
}

bool GJK_Alg::AddVertex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir, std::vector<glm::vec3> &Vertex)
{
	glm::vec3 NewVertex = Shape0->Support(Dir) - Shape1->Support(-Dir);
	Vertex.push_back(NewVertex);
	return glm::dot(NewVertex,Dir) > 0;
}

int GJK_Alg::EvolveSimplex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, std::vector<glm::vec3> &Vertex,glm::vec3 &Dir)
{
	int Size = Vertex.size();
	switch (Size)
	{
	case 0: {
		Dir = Shape1->GetPos() - Shape0->GetPos();
		
	}break;
	case 1: {
		Dir = -Dir;
	}break;
	case 2: {
		glm::vec3 AB = Vertex[1] - Vertex[0];
		glm::vec3 A0 = -Vertex[0];
		Dir = glm::cross(A0, AB);
		
	}break;
	case 3: {
		glm::vec3 AC = Vertex[2] - Vertex[0];
		glm::vec3 AB = Vertex[1] - Vertex[0];
		Dir = glm::cross(AC, AB);
		
		glm::vec3 A0 = -Vertex[0];
		if (glm::dot(Dir, A0) < 0)
			Dir = -Dir;
	}break;
	case 4: {
		glm::vec3 DA = Vertex[3] - Vertex[0];
		glm::vec3 DB = Vertex[3] - Vertex[1];
		glm::vec3 DC = Vertex[3] - Vertex[2];

		glm::vec3 D0 = -Vertex[3];

		glm::vec3 ABD_Norm = glm::cross(DB, DA);
		glm::vec3 BCD_Norm = glm::cross(DC, DB);
		glm::vec3 CAD_Norm = glm::cross(DA, DC);
		if (glm::dot(ABD_Norm, D0) > 0) {
			Vertex.erase(Vertex.begin() + 2);
			Dir = ABD_Norm;
		}else if (glm::dot(BCD_Norm, D0) > 0) {
			Vertex.erase(Vertex.begin() + 0);
			Dir = BCD_Norm;
		}else if (glm::dot(CAD_Norm, D0) > 0) {
			Vertex.erase(Vertex.begin() + 1);
			Dir = CAD_Norm;
		}
		else
		{
			return 2;
		}
	}break;
	default:
		break;
	}
	return AddVertex(Shape0,Shape1,Dir,Vertex) ? 0 : 1;
}

bool GJK_Alg::GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	int Result = 0;
	std::vector<glm::vec3> Vert;
	glm::vec3 Dir;
	int Count = 0;
	while (Result == 0)
	{
	Result = EvolveSimplex(Shape0, Shape1, Vert,Dir);
		Count++;
		if (Count == 20)
			return false;
	}
	if (Result == 1 )
		return false;
	return true;
}

GJK_Alg::GJK_Alg::GJK_Alg()
{
}

GJK_Alg::GJK_Alg::~GJK_Alg()
{
}
