#include "GJK.h"
using namespace CoatlPhysicsEngine;

float CoatlPhysicsEngine::F3Box(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	glm::vec3 N = glm::cross(A, B);
	return glm::dot(N, C);
}

glm::vec3 CoatlPhysicsEngine::TripleCross(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	glm::vec3 T = glm::cross(A, B);
	glm::vec3 Result = glm::cross(T, C);
	return Result;
}

bool CoatlPhysicsEngine::AddVertex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir, std::vector<glm::vec3> &Vertex)
{
	glm::vec3 NewVertex = Shape0->Support(Dir) - Shape1->Support(-Dir);
	Vertex.push_back(NewVertex);
	return glm::dot(NewVertex,Dir) >= 0;
}

int CoatlPhysicsEngine::EvolveSimplex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, std::vector<glm::vec3> &Vertex,glm::vec3 &Dir)
{
	int Size = Vertex.size();
	switch (Size)
	{
	case 0: {
		glm::vec3 t_Dir = Shape1->GetPos() - Shape0->GetPos();
		if (glm::dot(t_Dir, t_Dir) == 0)
		{
			Dir = t_Dir;
		}
		else
		{
			Dir = glm::normalize(t_Dir);
		}
	}break;
	case 1: {
		Dir = -Dir;
	}break;
	case 2: {
		glm::vec3 AB = Vertex[1] - Vertex[0];
		glm::vec3 A0 = -Vertex[0];
		glm::vec3 t_Dir = glm::cross(A0, AB);
		if (glm::dot(t_Dir, t_Dir) == 0)
		{
			Dir = t_Dir;
		}
		else
		{
			Dir = glm::normalize(t_Dir);
		}
	}break;
	case 3: {
		glm::vec3 AC = Vertex[2] - Vertex[0];
		glm::vec3 AB = Vertex[1] - Vertex[0];
		glm::vec3 t_Dir = glm::cross(AC, AB);
		if (glm::dot(t_Dir, t_Dir) == 0)
		{
			Dir = t_Dir;
		}
		else
		{
			Dir = glm::normalize(t_Dir);
		}
		glm::vec3 A0 = -Vertex[0];
		if (glm::dot(Dir, A0) < 0)
			Dir = -Dir;
	}break;
	case 4: {}break;
	default:
		break;
	}
	return AddVertex(Shape0,Shape1,Dir,Vertex) ? 0 : 1;
}

bool CoatlPhysicsEngine::GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	int Result = 0;
	std::vector<glm::vec3> Vert;
	glm::vec3 Dir;
	while (Result == 0)
	{
		Result = EvolveSimplex(Shape0, Shape1, Vert,Dir);
	}
	if (Result == 1 )
		return false;
	return true;
}
