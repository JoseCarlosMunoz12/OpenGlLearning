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
		if (glm::dot(Dir, Dir) != 0)
			Dir = glm::normalize(Dir);
	}break;
	case 1: {
		Dir = -Dir;
	}break;
	case 2: {
		glm::vec3 AB = Vertex[1] - Vertex[0];
		glm::vec3 A0 = -Vertex[0];
		Dir = glm::cross(A0, AB);
		if (glm::dot(Dir, Dir) != 0)
			Dir = glm::normalize(Dir);		
	}break;
	case 3: {
		glm::vec3 AC = Vertex[2] - Vertex[0];
		glm::vec3 AB = Vertex[1] - Vertex[0];
		Dir = glm::cross(AC, AB);
		if (glm::dot(Dir, Dir) != 0)
			Dir = glm::normalize(Dir);
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
		if (glm::dot(ABD_Norm, ABD_Norm) != 0)
			ABD_Norm = glm::normalize(ABD_Norm);
		glm::vec3 BCD_Norm = glm::cross(DC, DB);
		if (glm::dot(BCD_Norm, BCD_Norm) != 0)
			BCD_Norm = glm::normalize(BCD_Norm);
		glm::vec3 CAD_Norm = glm::cross(DA, DC);
		if (glm::dot(CAD_Norm, CAD_Norm) != 0)
			CAD_Norm = glm::normalize(CAD_Norm);
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

glm::vec3 GJK_Alg::EPA(std::vector<glm::vec3> Vertex, std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	glm::vec3 Faces[EPA_MAX_NUM_FACES][4];//Array of faces, each with 3 verts and a normal

	Faces[0][0] = Vertex[0];
	Faces[0][1] = Vertex[1];
	Faces[0][2] = Vertex[2];
	Faces[0][3] = glm::normalize(glm::cross(Vertex[1] - Vertex[0], Vertex[2] - Vertex[0])); //ABC
	Faces[1][0] = Vertex[0];
	Faces[1][1] = Vertex[2];
	Faces[1][2] = Vertex[3];
	Faces[1][3] = glm::normalize(glm::cross(Vertex[2] - Vertex[0], Vertex[3] - Vertex[0])); //ACD
	Faces[2][0] = Vertex[0];
	Faces[2][1] = Vertex[3];
	Faces[2][2] = Vertex[2];
	Faces[2][3] = glm::normalize(glm::cross(Vertex[3] - Vertex[0], Vertex[1] - Vertex[0])); //ADB
	Faces[3][0] = Vertex[1];
	Faces[3][1] = Vertex[3];
	Faces[3][2] = Vertex[2];
	Faces[3][3] = glm::normalize(glm::cross(Vertex[3] - Vertex[1], Vertex[2] - Vertex[1])); //BDC
	int Num_Face = 4;
	int ClosestFace;
	for (int It = 0; It < EPA_MAX_NUM_ITERATIONS; It++)
	{
		//Find Fce that's closest to origin
		float MinDist = glm::dot(Faces[0][0], Faces[0][3]);
		ClosestFace = 0;		
		for (int ii = 1 ; ii < Num_Face; ii++)
		{
			float dist = glm::dot(Faces[ii][0], Faces[ii][3]);
			if (dist < MinDist)
			{
				MinDist = dist;
				ClosestFace = ii;
			}
		}
		//search normal to face that's closets to origin
		glm::vec3 Search_Dir = Faces[ClosestFace][3];
		glm::vec3 P = Shape1->Support(Search_Dir) - Shape0->Support(-Search_Dir);
		float Dif = glm::dot(P, Search_Dir) - MinDist;
		if (Dif < EPA_TOLERANCE)
		{
			return Faces[ClosestFace][3] * glm::dot(P, Search_Dir);
		}
		glm::vec3 LooseEdges[EPA_MAX_NUM_FACES][2];
		int NumLooseEdge = 0;
		//finad all triangles that are facing P

	}
	return glm::vec3();
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
			return true;
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
