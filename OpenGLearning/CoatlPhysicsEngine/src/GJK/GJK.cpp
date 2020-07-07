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
	return  Result;
}

int GJK_Alg::Tr_Farthest_Point(std::vector<glm::vec3> Vec)
{
	glm::vec3 Zed = glm::vec3(0.f);
	float R = glm::distance(Vec[0], Zed);
	int Id = 0;
	int Count = 0;
	for (auto& ii : Vec)
	{
		float S = glm::distance(ii, Zed);
		if (R < S)
		{
			R = S;
			Id = Count;
		}
		Count++;
	}
	return Id;
}

glm::vec3 GJK_Alg::Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir)
{
	return Shape0->Support(Dir) - Shape1->Support(-Dir);
}

glm::vec3 GJK_Alg::EPA_Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir)
{
	return Shape0->EPA_Support(Dir) - Shape1->EPA_Support(-Dir);
}

glm::vec3 GJK_Alg::EPA(std::vector<glm::vec3> Vertex, std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	glm::vec3 Faces[EPA_MAX_NUM_FACES][4];//Array of faces, each with 3 verts and a normal	

	Faces[0][0] = Vertex[0];
	Faces[0][1] = Vertex[1];
	Faces[0][2] = Vertex[2];
	Faces[0][3] = glm::cross(Vertex[1] - Vertex[0], Vertex[2] - Vertex[0]); //ABC	
	Faces[1][0] = Vertex[0];
	Faces[1][1] = Vertex[2];
	Faces[1][2] = Vertex[3];
	Faces[1][3] = glm::cross(Vertex[2] - Vertex[0], Vertex[3] - Vertex[0]); //ACD	
	Faces[2][0] = Vertex[0];
	Faces[2][1] = Vertex[3];
	Faces[2][2] = Vertex[2];
	Faces[2][3] = glm::cross(Vertex[3] - Vertex[0], Vertex[1] - Vertex[0]); //ADB	
	Faces[3][0] = Vertex[1];
	Faces[3][1] = Vertex[3];
	Faces[3][2] = Vertex[2];
	Faces[3][3] = glm::cross(Vertex[3] - Vertex[1], Vertex[2] - Vertex[1]); //BDC	
	int Num_Face = 4;
	int ClosestFace;
	for (int It = 0; It < EPA_MAX_NUM_ITERATIONS; It++)
	{
		//Find Fce that's closest to origin	
		float MinDist = glm::dot(Faces[0][0], Faces[0][3]);
		ClosestFace = 0;
		for (int ii = 1; ii < Num_Face; ii++)
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
					glm::vec3 CurEdg[2] = { Faces[ii][jj],Faces[ii][(jj + 1) % 3] };
					bool FoundEdge = false;
					for (int kk = 0; kk < NumLooseEdge; kk++)
					{
						if (LooseEdges[kk][1] == CurEdg[0] && LooseEdges[kk][0] == CurEdg[1])
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
			Faces[Num_Face][3] = glm::cross(LooseEdges[ii][0] - LooseEdges[ii][1], LooseEdges[ii][0] - P);
			float bias = 0.000001f;
			float Diff = glm::dot(Faces[Num_Face][0], Faces[Num_Face][3]) + bias;
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

glm::vec3 GJK_Alg::C_F_E(std::vector<glm::vec3> Verts, std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
	glm::vec3 Dir)
{
	while (true)
	{
		if (Verts.size() == 3)
			break;
		Verts.push_back(EPA_Support(Shape0, Shape1, Dir));
		Simplex_Maker(Shape0, Shape1, Verts, Dir);
	}
	glm::vec3 Zed = glm::vec3(0.f);
	return MATH::ClosestPoint_Seg(Verts, Zed, Zed);
}

bool GJK_Alg::Simplex_Maker(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
	std::vector<glm::vec3>& Verts, glm::vec3& Dir)
{
	int Size = Verts.size();
	switch (Size)
	{
	case 0: {
		Dir = Shape0->GetPos() - Shape1->GetPos();
	}break;
	case 1: {
		Dir = -Verts[0];
	}break;
	case 2: {
		glm::vec3 AB = Verts[0] - Verts[1];
		glm::vec3 A0 = -Verts[1];
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
		glm::vec3 A0 = -A;
		glm::vec3 ABC = glm::cross(AB, AC);

		glm::vec3 ABC_AC = glm::cross(ABC, AC);
		glm::vec3 AB_ABC = glm::cross(AB, ABC);

		if (glm::dot(ABC_AC, A0) > 0.f)
		{
			if (glm::dot(AC, A0) > 0.f)
				Dir = TripleCross(AC, A0);
			else
			{
				if(glm::dot(AB, A0) > 0.f)
					Dir = TripleCross(AB, A0);
				else
					Dir = A0;
			}
		}
		else
		{
			if (glm::dot(AB_ABC,A0) > 0.f)
			{
				if (glm::dot(AB, A0) > 0.f)
					Dir = TripleCross(AB, A0);
				else
					Dir = A0;
			}
			else
			{
				if (glm::dot(ABC, A0) > 0.f)
					Dir = ABC;
				else
					Dir = -ABC;
			}
		}
	}break;
	case 4: {
		glm::vec3 A = Verts[3];
		glm::vec3 B = Verts[2];
		glm::vec3 C = Verts[1];
		glm::vec3 D = Verts[0];

		glm::vec3 AC = C - A;
		glm::vec3 AD = D - A;
		glm::vec3 AB = B - A;
		glm::vec3 A0 = -A;

		glm::vec3 AC_AB = glm::cross(AC, AB);
		glm::vec3 AB_AD = glm::cross(AB, AD);
		glm::vec3 AD_AC = glm::cross(AD, AC);

		float vABC = glm::dot(AC_AB,A0);
		float vABD = glm::dot(AB_AD,A0);
		float vADC = glm::dot(AD_AC,A0);

		int neg = 0;
		int pos = 0;

		if (vABC > 0)
			pos++;
		else
			neg++;

		if (vABD > 0)
			pos++;
		else
			neg++;

		if (vADC > 0)
			pos++;
		else
			neg++;

		if (neg == 3)
			return true;
		if (pos == 3)
			return true;
		if (neg == 2 and pos == 1)
		{
			if (vADC > 0.f)
			{
				Verts.erase(Verts.begin() + 1);
				Dir = AD_AC;
			}
			else if (vABD > 0.f)
			{
				Verts.erase(Verts.begin() + 2);
				Dir = AB_AD;
			}
			else
			{
				Verts.erase(Verts.begin() +  0);
				Dir = AC_AB;
			}
		}
		else
		{
			if (vADC < 0.f)
			{
				Verts.erase(Verts.begin() + 1);
				Dir = -AD_AC;
			}
			else if (vABD < 0.f)
			{
				Verts.erase(Verts.begin() + 2);
				Dir = -AB_AD;
			}
			else
			{
				Verts.erase(Verts.begin() + 0);
				Dir = -AC_AB;
			}
		}
	}break;
	default:
		break;
	}
	return false;
}

std::vector<int> GJK_Alg::C_F_S(std::vector<glm::vec3>& Verts)
{
	glm::vec3 A = Verts[3];
	glm::vec3 B = Verts[2];
	glm::vec3 C = Verts[1];
	glm::vec3 D = Verts[0];

	glm::vec3 AC = C - A;
	glm::vec3 AD = D - A;
	glm::vec3 AB = B - A;
	glm::vec3 A0 = -A;

	glm::vec3 AC_AB = glm::cross(AC, AB);
	glm::vec3 AB_AD = glm::cross(AB, AD);
	glm::vec3 AD_AC = glm::cross(AD, AC);

	float vABC = glm::dot(AC_AB, A0);
	float vABD = glm::dot(AB_AD, A0);
	float vADC = glm::dot(AD_AC, A0);

	int neg = 0;
	int pos = 0;

	if (vABC > 0)
		pos++;
	else
		neg++;

	if (vABD > 0)
		pos++;
	else
		neg++;

	if (vADC > 0)
		pos++;
	else
		neg++;
	if (neg == 2 and pos == 1)
	{
		if (vADC > 0.f)
			return { 0, 2 };
		else if (vABD > 0.f)
			return { 0, 1 };
		else
			return { 1, 2 };
	}
	else
	{
		if (vADC < 0.f)
			return { 0, 2 };
		else if (vABD < 0.f)
			return { 0, 1 };
		else
			return { 1, 2 };
	}
}

bool GJK_Alg::GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	std::vector<glm::vec3> Verts;
	glm::vec3 Dir;
	Simplex_Maker(Shape0, Shape1,Verts, Dir);
	Verts.push_back(Support(Shape0, Shape1, Dir));
	Simplex_Maker(Shape0, Shape1, Verts, Dir);
	for (int ii = 0; ii < 20; ii++)
	{
		glm::vec3 A = Support(Shape0, Shape1, Dir);
		if (glm::dot(A, Dir) <= 0.f)
			return false;
		Verts.push_back(A);
		if (Simplex_Maker(Shape0, Shape1, Verts, Dir))
			return true;
	}
	return false;
}

bool GJK_Alg::EPA_GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3& DistVec)
{
	std::vector<glm::vec3> Verts;
	glm::vec3 Dir;
	Simplex_Maker(Shape0, Shape1, Verts, Dir);
	Verts.push_back(EPA_Support(Shape0, Shape1, Dir));
	Simplex_Maker(Shape0, Shape1, Verts, Dir);
	bool Col = false;
	for (int ii = 0; ii < 20; ii++)
	{
		glm::vec3 A = EPA_Support(Shape0, Shape1, Dir);
		if (glm::dot(A, Dir) <= 0.f)
			break;
		Verts.push_back(A);
		if (Simplex_Maker(Shape0, Shape1, Verts, Dir))
		{
			Col = true;
			break;
		}
	}
	if (Col)
		Dir = EPA(Verts, Shape0, Shape1);
	else
		DistVec = C_F_E(Verts, Shape0, Shape1, Dir);
	return Col;
}

GJK_Alg::GJK_Alg::GJK_Alg()
{
}

GJK_Alg::GJK_Alg::~GJK_Alg()
{
}
