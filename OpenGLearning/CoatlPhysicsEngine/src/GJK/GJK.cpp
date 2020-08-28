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
		float S = glm::dot(ii, ii);
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

glm::vec3 GJK_Alg::Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
	std::vector<glm::vec3> Seg0,std::vector<glm::vec3> Seg1, glm::vec3 Dir)
{
	glm::vec3 Sp0 = Support(Seg0, Dir) + Shape0->Support(Dir);
	glm::vec3 Sp1 = Support(Seg1, -Dir) + Shape1->Support(-Dir);
	return Sp0 - Sp1;
}

glm::vec3 GJK_Alg::Support(std::vector<glm::vec3> Seg, glm::vec3 Dir)
{
	float S = glm::dot(Seg[0], Dir);
	glm::vec3 MaxPnt = Seg[0];
	int Size = Seg.size();
	for (int ii = 1; ii < Size; ii++)
	{
		float T = glm::dot(Seg[ii], Dir);
		if (T > S)
		{
			S = T;
			MaxPnt = Seg[ii];
		}
	}
	return MaxPnt;
}

bool GJK_Alg::Simplex_Maker(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Pos0, glm::vec3 Pos1,
	std::vector<glm::vec3>& Verts, glm::vec3& Dir)
{
	int Size = Verts.size();
	switch (Size)
	{
	case 0: {
		glm::vec3 S_Pos0 = Shape0->GetPos() + Pos0;
		glm::vec3 S_Pos1 = Shape1->GetPos() + Pos1;
		Dir = S_Pos0 - S_Pos1;
		Dir = glm::vec3(1.f);
	}break;
	case 1: {
		Dir = -Verts[0];
	}break;
	case 2: {
		glm::vec3 AB = Verts[0] - Verts[1];
		glm::vec3 A0 = -Verts[1];
		if (glm::dot(AB, A0) > 0.f)
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
				if (glm::dot(AB, A0) > 0.f)
					Dir = TripleCross(AB, A0);
				else
					Dir = A0;
			}
		}
		else
		{
			if (glm::dot(AB_ABC, A0) > 0.f)
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

		float vABC = glm::dot(AC_AB, A0);
		float vABD = glm::dot(AB_AD, A0);
		float vADC = glm::dot(AD_AC, A0);

		int neg = 0;
		int pos = 0;

		if (vABC >= 0)
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
				Verts.erase(Verts.begin() + 0);
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

glm::vec3 GJK_Alg::EPA_Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir)
{
	return Shape0->EPA_Support(Dir) - Shape1->EPA_Support(-Dir);
}

glm::vec3 GJK_Alg::EPA(std::vector<glm::vec3> Vertex, std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, float& Pen)
{
	glm::vec3 Faces[EPA_MAX_NUM_FACES][4];//Array of faces, each with 3 verts and a normal	
	//A = 3
	//B = 2
	//C = 1
	//D = 0
	Faces[0][0] = Vertex[3];
	Faces[0][1] = Vertex[2];
	Faces[0][2] = Vertex[1];
	Faces[0][3] = glm::normalize(glm::cross(Vertex[2] - Vertex[3], Vertex[1] - Vertex[3])); //ABC	
	Faces[1][0] = Vertex[3];
	Faces[1][1] = Vertex[1];
	Faces[1][2] = Vertex[0];
	Faces[1][3] = glm::normalize(glm::cross(Vertex[1] - Vertex[3], Vertex[0] - Vertex[3])); //ACD	
	Faces[2][0] = Vertex[3];
	Faces[2][1] = Vertex[0];
	Faces[2][2] = Vertex[2];
	Faces[2][3] = glm::normalize(glm::cross(Vertex[0] - Vertex[3], Vertex[2] - Vertex[3])); //ADB	
	Faces[3][0] = Vertex[2];
	Faces[3][1] = Vertex[0];
	Faces[3][2] = Vertex[1];
	Faces[3][3] = glm::normalize(glm::cross(Vertex[0] - Vertex[2], Vertex[1] - Vertex[2])); //BDC	
	int Num_Face = 4;
	int ClosestFace;
	glm::vec3 Zed(0.f);
	for (int It = 0; It < EPA_MAX_NUM_ITERATIONS; It++)
	{
		//Find Face that's closest to origin
		float Min_Dist = MATH::Distance_Tr_Pnt({ Faces[0][0] ,Faces[0][1] ,Faces[0][2] },Zed, Zed);
		ClosestFace = 0;
		for (int ii = 1; ii < Num_Face; ii++)
		{
			float dist = MATH::Distance_Tr_Pnt({ Faces[ii][0] ,Faces[ii][1] ,Faces[ii][2] }, Zed, Zed);
			if (dist < Min_Dist)
			{
				Min_Dist = dist; 
				ClosestFace = ii;
			}
		}
		//search normal to face that's closets to origin	
		glm::vec3 Search_Dir = Faces[ClosestFace][3];
		glm::vec3 P = EPA_Support(Shape0, Shape1, Search_Dir);
		float Dif = glm::dot(P, Search_Dir) - Min_Dist;
		if (Dif < EPA_TOLERANCE)
		{
			Pen = MATH::Distance_Tr_Pnt({ Faces[ClosestFace][0] ,Faces[ClosestFace][1] ,Faces[ClosestFace][2] }, Zed, Zed);
			return Faces[ClosestFace][3];
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
			Faces[Num_Face][3] = glm::normalize(glm::cross(LooseEdges[ii][0] - LooseEdges[ii][1], LooseEdges[ii][0] - P));
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
	Pen = glm::dot(Faces[ClosestFace][0], Faces[ClosestFace][3]);
	return Faces[ClosestFace][3] ;
}

glm::vec3 GJK_Alg::ClosestPoint(std::vector<glm::vec3> Verts)
{
	glm::vec3 Zed = glm::vec3(0.f);
	switch (Verts.size())
	{
	case 1:
		return Verts[0];
		break;
	case 2:
		return MATH::ClosestPoint_Seg(Verts, Zed);
		break;
	case 3:
		return MATH::ClosestPoint_Seg(Verts, Zed, Zed);
		break;
	default:
		break;
	}
	return Zed;
}

std::vector<glm::vec3> GJK_Alg::NewSimplex(std::vector<glm::vec3> Verts, glm::vec3 NewPoint)
{
	int Vec[6] = {0,1,0,2,1,2};
	int Vals = 0;
	glm::vec3 Zed = glm::vec3(0.f);
	glm::vec3 Dis = MATH::ClosestPoint_Seg({Verts[0],Verts[1],NewPoint}, Zed, Zed);
	float P_Dis = glm::distance(Zed, Dis);
	for (int ii = 1; ii < 3; ii++)
	{
		glm::vec3 NewDis = MATH::ClosestPoint_Seg({ Verts[Vec[2*ii]],Verts[Vec[2* ii +1]],NewPoint }, Zed, Zed);
		float T_Dis = glm::distance(Zed, NewDis);
		if (T_Dis < P_Dis)
		{
			P_Dis = T_Dis;
			Vals = ii;
		}
	}
	return {Verts[Vec[2 * Vals]], Verts[Vec[2*Vals + 1]], NewPoint};
}

float GJK_Alg::Cl_Dist(std::vector<glm::vec3> Verts)
{
	glm::vec3 Cl_P = ClosestPoint(Verts);
	return glm::distance(glm::vec3(0.f), Cl_P);
}

glm::vec3 GJK_Alg::C_F_E( std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	std::vector<glm::vec3> Verts; 
	glm::vec3 NewDir;
	Simplex_Maker(Shape0, Shape1, Verts, NewDir);
	Verts.push_back(EPA_Support(Shape0, Shape1,NewDir));
	glm::vec3 Zed = glm::vec3(0.f);
	NewDir = this->ClosestPoint(Verts);
	float Cl_Dis = glm::distance(Zed, NewDir);
	glm::vec3 A = EPA_Support(Shape0, Shape1, -NewDir);
	for(int ii = 0; ii < 20; ii++)
	{
		if (std::find(Verts.begin(), Verts.end(), A) != Verts.end())
			break;
		if (Verts.size() == 3)
			Verts = this->NewSimplex(Verts, A);
		else
			Verts.push_back(A);
		NewDir = this->ClosestPoint(Verts);
		float Cur_Dis = glm::distance(Zed, NewDir);
		if (Cur_Dis < Cl_Dis)
			Cl_Dis = Cur_Dis;
		else if ((Cur_Dis - Cl_Dis) < 0.00001f)
			break;
		A = EPA_Support(Shape0, Shape1, -NewDir);
	}
	return this->ClosestPoint(Verts);
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

		if (vABC >= 0)
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
		if (std::find(Verts.begin(), Verts.end(), A) != Verts.end())
			break;
		Verts.push_back(A);
		if (Simplex_Maker(Shape0, Shape1, Verts, Dir))
			return true;
	}
	return false;
}

bool GJK_Alg::GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
	std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1)
{
	std::vector<glm::vec3> Verts;
	glm::vec3 Pos0 = (Seg0[1] + Seg0[0]) / 2.f;
	glm::vec3 Pos1 = (Seg1[1] + Seg1[0]) / 2.f;
	glm::vec3 Dir;
	Simplex_Maker(Shape0, Shape1,Pos0, Pos1, Verts, Dir);
	Verts.push_back(Support(Shape0, Shape1,Seg0, Seg1, Dir));
	Simplex_Maker(Shape0, Shape1, Pos0, Pos1, Verts, Dir);
	for (int ii = 0; ii < 20; ii++)
	{
		glm::vec3 A = Support(Shape0, Shape1, Seg0, Seg1, Dir);
		if (glm::dot(A, Dir) <= 0.f)
			return false;
		if (std::find(Verts.begin(), Verts.end(), A) != Verts.end())
			break;
		Verts.push_back(A);
		if (Simplex_Maker(Shape0, Shape1, Pos0, Pos1, Verts, Dir))
			return true;
	}
	return false;
}

bool GJK_Alg::EPA_GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3& DistVec, float& Pen)
{
	std::vector<glm::vec3> Verts;
	glm::vec3 Dir;
	Simplex_Maker(Shape0, Shape1, Verts, Dir);
	Verts.push_back(EPA_Support(Shape0, Shape1, Dir));
	Simplex_Maker(Shape0, Shape1, Verts, Dir);
	bool Col = false;
	glm::vec3 Zed = glm::vec3(0.f);
	for (int ii = 0; ii < 20; ii++)
	{
		glm::vec3 A = EPA_Support(Shape0, Shape1, Dir);
		if (glm::dot(A, Dir) <= 0.f)
			break;
		if (std::find(Verts.begin(), Verts.end(), A) != Verts.end())
			break;
		Verts.push_back(A);
		Col = Simplex_Maker(Shape0, Shape1, Verts, Dir);
		if (Col)
			break;
	}

	if (Col)
		DistVec = -EPA(Verts, Shape0, Shape1,Pen);
	else
	{
		DistVec = C_F_E(Shape0, Shape1);
		Pen = glm::distance(Zed, DistVec);
	}
	if (DistVec != glm::vec3(0.f))
		DistVec = glm::normalize(DistVec);
	return Col;
}

GJK_Alg::GJK_Alg::GJK_Alg()
{
}

GJK_Alg::GJK_Alg::~GJK_Alg()
{
}
