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

glm::vec3 GJK_Alg::EPA_Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir)
{
	return Shape0->EPA_Support(Dir) - Shape1->EPA_Support(-Dir);
}

glm::vec3 GJK_Alg::EPA(std::vector<glm::vec3> Verts, std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	return glm::vec3();
}

glm::vec3 GJK_Alg::C_F_E(std::vector<glm::vec3> Verts, std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1)
{
	return glm::vec3();
}

bool GJK_Alg::Simplex_Maker(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
	std::vector<glm::vec3>& Verts, glm::vec3& Dir)
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
		glm::vec3 A0 = -A;
		glm::vec3 ABC = glm::cross(AB, AC);

		glm::vec3 ABC_AC = glm::cross(ABC, AC);
		glm::vec3 AB_ABC = glm::cross(AB, ABC);

		if (glm::dot(ABC_AC, AC) > 0.f)
		{
			if (glm::dot(AC, AB) > 0.f)
			{
				Dir = TripleCross(AC, A0);
			}
			else
			{
				if(glm::dot(AB, A0) > 0.f)
				{
					Dir = TripleCross(AB, A0);
				}
				else
				{
					Dir = MATH::Normalize(A0);
				}
			}
		}
		else
		{
			if (glm::dot(AB_ABC,A0) > 0.f)
			{
				if (glm::dot(AB, A0) > 0.f)
				{
					Dir = TripleCross(AB, A0);
				}
				else
				{
					Dir = MATH::Normalize(A0);
				}
			}
			else
			{
				if (glm::dot(ABC, A0) > 0.f)
				{
					Dir = MATH::Normalize(ABC);
				}
				else
				{
					Dir = -MATH::Normalize(ABC);
				}
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
				Verts.erase(Verts.begin() + 3);
				Dir = MATH::Normalize(AD_AC);
			}
			else if (vABD > 0.f)
			{
				Verts.erase(Verts.begin() + 2);
				Dir = MATH::Normalize(AB_AD);
			}
			else
			{
				Verts.erase(Verts.begin() +  1);
				Dir = MATH::Normalize(AC_AB);
			}
		}
		else
		{
			if (vADC < 0.f)
			{
				Verts.erase(Verts.begin() + 3);
				Dir = -MATH::Normalize(AD_AC);
			}
			else if (vABD < 0.f)
			{
				Verts.erase(Verts.begin() + 2);
				Dir = -MATH::Normalize(AB_AD);
			}
			else
			{
				Verts.erase(Verts.begin() + 1);
				Dir = -MATH::Normalize(AC_AB);
			}
		}
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
	for (int ii = 0; ii < 124; ii++)
	{
		glm::vec3 A = Support(Shape0, Shape1, Dir);
		if (glm::dot(A, Dir) < 0.f)
			return false;
		Verts.push_back(A);
		if (Simplex_Maker(Shape0, Shape1, Verts, Dir))
			return true;
	}
	return true;
}

bool GJK_Alg::EPA_GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3& DistVec)
{

	std::vector<glm::vec3> Verts;
	glm::vec3 Dir;
	Simplex_Maker(Shape0, Shape1, Verts, Dir);
	Verts.push_back(EPA_Support(Shape0, Shape1, Dir));
	Simplex_Maker(Shape0, Shape1, Verts, Dir);
	bool Col = false;
	for (int ii = 0; ii < 64; ii++)
	{
		glm::vec3 A = EPA_Support(Shape0, Shape1, Dir);
		if (glm::dot(A, Dir) < 0.f)
			break;
		Verts.push_back(A);
		if (Simplex_Maker(Shape0, Shape1, Verts, Dir))
		{
			Col = true;
			break;
		}
	}
	if (Col)
		DistVec = EPA(Verts, Shape0, Shape1);
	else
		DistVec = C_F_E(Verts, Shape0, Shape1);
	return Col;
}

GJK_Alg::GJK_Alg::GJK_Alg()
{
}

GJK_Alg::GJK_Alg::~GJK_Alg()
{
}
