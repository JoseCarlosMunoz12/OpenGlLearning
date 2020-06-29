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
	return Result;
}

bool GJK_Alg::AddVertex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir, std::vector<glm::vec3> &Vertex)
{
	glm::vec3 NewVertex = Shape0->Support(Dir) - Shape1->Support(-Dir);
	Vertex.push_back(NewVertex);
	if (glm::dot(NewVertex, NewVertex) != 0.f)
		NewVertex = glm::normalize(NewVertex);
	return glm::dot(NewVertex,Dir) >= 0.f;
}

bool GJK_Alg::EPA_AddVertex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir, std::vector<glm::vec3>& Vertex)
{
	glm::vec3 NewVertex = Shape0->EPA_Support(Dir) - Shape1->EPA_Support(-Dir);
	Vertex.push_back(NewVertex);
	if (glm::dot(NewVertex, NewVertex) != 0.f)
		NewVertex = glm::normalize(NewVertex);
	float val = glm::dot(NewVertex, Dir);
	return  val >= 0.f;
}

int GJK_Alg::EvolveSimplex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, std::vector<glm::vec3> &Vertex,glm::vec3 &Dir)
{
	int Size = Vertex.size();
	switch (Size)
	{
	case 0: {
		Dir = Shape1->GetPos() - Shape0->GetPos();
		if (glm::dot(Dir, Dir) != 0.f)
			Dir = glm::normalize(Dir);
	}break;
	case 1: {
		Dir = -Dir;
	}break;
	case 2: {
		glm::vec3 AB = Vertex[1] - Vertex[0];
		glm::vec3 A0 = -Vertex[0];
		Dir = this->TripleCross(AB,A0);
		if (glm::dot(Dir, Dir) != 0.f)
			Dir = glm::normalize(Dir);
	}break;
	case 3: {
		glm::vec3 AC = Vertex[2] - Vertex[0];
		glm::vec3 AB = Vertex[1] - Vertex[0];
		Dir = glm::cross(AB, AC);
		if (glm::dot(Dir, Dir) != 0.f)
			Dir = glm::normalize(Dir);
		glm::vec3 A0 = -Vertex[0];
		if (glm::dot(Dir, A0) < 0)
			Dir = -Dir;
	}break;
	case 4: {
		glm::vec3 DA = Vertex[3] - Vertex[0];
		glm::vec3 DB = Vertex[3] - Vertex[1];
		glm::vec3 DC = Vertex[3] - Vertex[2];

		glm::vec3 D0 = -glm::normalize(Vertex[3]);

		glm::vec3 ABD_Norm = glm::cross(DB, DA);
		glm::vec3 BCD_Norm = glm::cross(DC, DB);
		glm::vec3 CAD_Norm = glm::cross(DA, DC);
		if (glm::dot(ABD_Norm, D0) >= 0) {
			Vertex.erase(Vertex.begin() + 2);
			Dir = ABD_Norm;
		}else if (glm::dot(BCD_Norm, D0) >= 0) {
			Vertex.erase(Vertex.begin() + 0);
			Dir = BCD_Norm;
		}else if (glm::dot(CAD_Norm, D0) >= 0) {
			Vertex.erase(Vertex.begin() + 1);
			Dir = CAD_Norm;
		}
		else
		{
			return 2;
		}
		if (glm::dot(Dir, Dir) != 0.f)
			Dir = glm::normalize(Dir);
	}break;
	default:
		break;
	}
	return AddVertex(Shape0,Shape1,Dir,Vertex) ? 0 : 1;
}

int GJK_Alg::EPA_EvolveSimplex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, std::vector<glm::vec3>& Vertex, glm::vec3& Dir)
{
	int Size = Vertex.size();
	switch (Size)
	{
	case 0: {
		Dir = Shape1->GetPos() - Shape0->GetPos();
		if (glm::dot(Dir, Dir) != 0.f)
			Dir = glm::normalize(Dir);
	}break;
	case 1: {
		Dir = -Dir;
	}break;
	case 2: {
		glm::vec3 AB = Vertex[1] - Vertex[0];
		if (glm::dot(AB, AB) != 0.f)
			AB = glm::normalize(AB);
		glm::vec3 A0 = -Vertex[0];
		if (glm::dot(A0, A0) != 0.f)
			A0 = glm::normalize(A0);
		Dir = this->TripleCross(AB, A0);
		if (glm::dot(Dir, Dir) != 0.f)
			Dir = glm::normalize(Dir);
	}break;
	case 3: {
		glm::vec3 AC = Vertex[2] - Vertex[0];
		if (glm::dot(AC, AC) != 0.f)
			AC = glm::normalize(AC);
		glm::vec3 AB = Vertex[1] - Vertex[0];
		if (glm::dot(AB, AB) != 0.f)
			AB = glm::normalize(AB);
		Dir = glm::cross(AC, AB);
		glm::vec3 A0 = -Vertex[0];
		if (glm::dot(Dir, A0) < 0)
			Dir = -Dir;
	}break;
	case 4: {
		glm::vec3 DA = Vertex[3] - Vertex[0];
		if (glm::dot(DA, DA) != 0.f)
			DA = glm::normalize(DA);
		glm::vec3 DB = Vertex[3] - Vertex[1];
		if (glm::dot(DB, DB) != 0.f)
			DB = glm::normalize(DB);
		glm::vec3 DC = Vertex[3] - Vertex[2];
		if (glm::dot(DC, DC) != 0.f)
			DC = glm::normalize(DC);

		glm::vec3 D0 = -glm::normalize(Vertex[3]);

		glm::vec3 ABD_Norm = glm::cross(DB, DA);
		glm::vec3 BCD_Norm = glm::cross(DC, DB);
		glm::vec3 CAD_Norm = glm::cross(DA, DC);
		if (glm::dot(ABD_Norm, D0) > 0) {
			Vertex.erase(Vertex.begin() + 2);
			Dir = ABD_Norm;
		}
		else if (glm::dot(BCD_Norm, D0) > 0) {
			Vertex.erase(Vertex.begin() + 0);
			Dir = BCD_Norm;
		}
		else if (glm::dot(CAD_Norm, D0) > 0) {
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
	return EPA_AddVertex(Shape0, Shape1, Dir, Vertex) ? 0 : 1;
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
		glm::vec3 P = Shape1->EPA_Support(Search_Dir) - Shape0->EPA_Support(-Search_Dir);
		float Dif = glm::dot(P, Search_Dir) - MinDist;
		if (Dif < EPA_TOLERANCE)
		{
			float S = glm::dot(P, Search_Dir);
			return Faces[ClosestFace][3] * S;
		}
		glm::vec3 LooseEdges[EPA_MAX_NUM_FACES][2];
		int NumLooseEdge = 0;
		//find all triangles that are facing P
		for (int ii = 0; ii < Num_Face; ii++)
		{
			if (glm::dot(Faces[ii][3], P - Faces[ii][0]) > 0)
			{
				for (int jj = 0; jj < 3; jj++)
				{
					glm::vec3 CurEdg[2] = {Faces[ii][jj],Faces[ii][(jj +1)%3]};
					bool FoundEdge = false;
					for (int kk = 0; kk < NumLooseEdge; kk++)
					{
						if(LooseEdges[kk][1] == CurEdg[0] && LooseEdges[kk][0] == CurEdg[1])
						{
							LooseEdges[kk][0] = LooseEdges[NumLooseEdge - 1][0];
							LooseEdges[kk][1] = LooseEdges[NumLooseEdge - 1][1];
							NumLooseEdge--;
							FoundEdge = true;
							kk = NumLooseEdge;
						}
					}
					if (!FoundEdge)
					{
						if (NumLooseEdge >= EPA_MAX_NUM_LOOSE_EDGES)break;
						LooseEdges[NumLooseEdge][0] = CurEdg[0];
						LooseEdges[NumLooseEdge][1] = CurEdg[1];
						NumLooseEdge++;
					}
				}
				Faces[ii][0] = Faces[Num_Face - 1][0];
				Faces[ii][1] = Faces[Num_Face - 1][1];
				Faces[ii][2] = Faces[Num_Face - 1][2];
				Faces[ii][3] = Faces[Num_Face - 1][3];
				Num_Face--;
				ii--;
			}
		}
		//Reconstruct polytope with p added
		for (int ii = 0; ii < NumLooseEdge; ii++)
		{
			if (Num_Face >= EPA_MAX_NUM_FACES) break;
			Faces[Num_Face][0] = LooseEdges[ii][0];
			Faces[Num_Face][1] = LooseEdges[ii][1];
			Faces[Num_Face][2] = P;
			Faces[Num_Face][3] = glm::normalize(glm::cross(LooseEdges[ii][0] - LooseEdges[ii][1], LooseEdges[ii][0] - P));
			float bias = 0.000001f;
			float Diff = glm::dot(Faces[Num_Face][0],Faces[Num_Face][3]) + bias;
			if (Dif < 0)
			{
				glm::vec3 Temp = Faces[Num_Face][0];
				Faces[Num_Face][0] = Faces[Num_Face][1];
				Faces[Num_Face][1] = Temp;
				Faces[Num_Face][3] = -Faces[Num_Face][3];
			}
			Num_Face++;
		}
	}
	float G = glm::dot(Faces[ClosestFace][0], Faces[ClosestFace][3]);
	return Faces[ClosestFace][3] * G;
}

glm::vec3 GJK_Alg::DistToOrigin(std::vector<glm::vec3> Vert, glm::vec3 Dir)
{
	glm::vec3 Zed = glm::vec3(0.f);
	int Size = Vert.size();
		int rmv = 0;
	if (Size != 3)
	{
		float S = glm::dot(Vert[0], Dir);
		glm::vec3 MaxPnt = Vert[0];
		for (int ii = 1; ii < Size; ii++)
		{
			float T = glm::dot(Vert[ii], Dir);
			if (T > S)
			{
				S = T;
				rmv = ii;
			}
		}
	}
	Vert.erase(Vert.begin() + rmv);
	Plane PL(Vert);
	float t = PL.D;
	return Zed - t * PL.Normal;

}

bool GJK_Alg::GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	int Result = 0;
	std::vector<glm::vec3> Vert;
	glm::vec3 Dir;
	int Count = 0;
	EvolveSimplex(Shape0, Shape1, Vert, Dir);
	while (Result == 0)
	{
		Result = EvolveSimplex(Shape0, Shape1, Vert,Dir);
		Count++;
		if (Count == 40)
		{
			return false;
		}
	}
	if (Result == 1 )
		return false;
	return true;
}

bool GJK_Alg::EPA_GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3& DistVec)
{
	int Result = 0;
	std::vector<glm::vec3> Vert;
	glm::vec3 Dir;
	glm::vec3 InitDir;
	int Count = 0;
	EPA_EvolveSimplex(Shape0, Shape1, Vert, InitDir);
	Dir = InitDir;
	while (Result == 0)
	{
		Result = EPA_EvolveSimplex(Shape0, Shape1, Vert, Dir);
		Count++;
		if (Count == 40)
		{
			break;
		}
	}
	if (Result == 2){
		DistVec = EPA(Vert, Shape0, Shape1);
		return true;
	}
	DistVec = this->DistToOrigin(Vert, InitDir);
	return false;
}

GJK_Alg::GJK_Alg::GJK_Alg()
{
}

GJK_Alg::GJK_Alg::~GJK_Alg()
{
}
