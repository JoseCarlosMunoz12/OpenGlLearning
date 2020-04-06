#include "CapsuleColAABB.h"
using namespace CoatlPhysicsEngine;

float CapsuleColAABB::ClosestPoint(std::vector<glm::vec3> Cap,std::vector<glm::vec3> Segment)
{
	float EPSILON = 0.00000001;

	glm::vec3 D1 = Cap[0] - Cap[1];
	glm::vec3 D2 = Segment[0] - Segment[1];
	glm::vec3 R = Cap[1] - Segment[1];

	float A = glm::dot(D1, D1);
	float E = glm::dot(D2, D2);
	float F = glm::dot(D2, R);

	float S, T;
	glm::vec3 C1;
	glm::vec3 C2;
	if (A <= EPSILON && E <= EPSILON)
	{
		S = 0.f;
		T = 0.f;
		C1 = Cap[1];
		C2 = Segment[1];
		return glm::dot(C1 -C2, C1 - C2);
	}

	if (A <= EPSILON)
	{
		S = 0.f;
		T = F / E;
		T = glm::clamp(T, 0.f,1.f);
	}
	else
	{
		float C = glm::dot(D1,R);
		if (E <= EPSILON)
		{
			T = 0.f;
			S = glm::clamp(-C / A ,0.f,1.f);
		}
		else
		{
			float B = glm::dot(D1, D2);
			float Denom = A * E - B * B;
			if (Denom != 0.f)
			{
				S = glm::clamp((B *F - C * E), 0.f, 1.f);
			}
			else
			{
				S = 0.f;
			}
			T = (B * S + F) / E;

			if (T < 0.f)
			{
				T = 0.f;
				S = glm::clamp(-C /A ,0.f,1.f);
			}
			else if(T > 1.f)
			{
				T = 1.f;
				S = glm::clamp((B - C) /A, 0.f, 1.f);
			}
		}
	}

	C1 = Cap[1] + D1 * S;
	C2 = Segment[1] + D2 * T;

	return glm::dot(C1 - C2, C1 - C2);
}

bool CapsuleColAABB::CapColAABB(Capsule Cap, AABB_Obj Obj)
{
	std::vector<int> ID = Obj.GetSegmentsID();
	int Size = ID.size() / 2;
	glm::vec3 Pos = Obj.GetPos();
	std::vector<glm::vec3> Points = Obj.GetPoints();
	for (auto& jj : Points)
	{
		jj = jj + Pos;
	}
	float R = Cap.GetRadius();
	R = R * R;
	for (int ii = 0; ii < Size; ii++)
	{
		int JJ = ii * 2;
		int KK = JJ + 1;
		float Dis = ClosestPoint(Cap.GetSegment(), {Points[ID[JJ]], Points[ID[KK]]});
		if (R > Dis)
		{
			return true;
		}
	}
	return false;
}

CapsuleColAABB::CapsuleColAABB()
{
}

CapsuleColAABB::~CapsuleColAABB()
{
}
