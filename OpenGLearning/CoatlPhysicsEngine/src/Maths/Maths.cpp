#include "Maths.h"

glm::vec3 MATH::Set()
{
	return glm::vec3(1.f);
}

glm::vec3 MATH::ClosetPoint_Pln(Plane Pl, glm::vec3 Point)
{
	float T = glm::dot(Pl.Normal, Point);
	return  Point + T * Pl.Normal;
}

glm::vec3 MATH::ClosestPoint_Seg(std::vector<glm::vec3> triangle, glm::vec3 Pos, glm::vec3 Point)
{
	glm::vec3 AB = triangle[1] - triangle[0];
	glm::vec3 AC = triangle[2] - triangle[0];
	glm::vec3 AP = Point - (triangle[0] + Pos);
	float D1 = glm::dot(AB, AP);
	float D2 = glm::dot(AC, AP);
	if (D1 <= 0.f && D2 <= 0.f)
		return (triangle[0] + Pos);

	glm::vec3 BP = Point - (triangle[1] + Pos);
	float D3 = glm::dot(AB, BP);
	float D4 = glm::dot(AC, BP);
	if (D3 >= 0.f && D4 <= D3)
		return (triangle[1] + Pos);

	float VC = D1 * D4 - D3 * D2;
	if (VC <= 0.f && D1 >= 0.f && D3 <= 0.f)
	{
		float V = D1 / (D1 - D3);
		return Pos + triangle[0] + V * AB;
	}

	glm::vec3 CP = Point - (triangle[2] + Pos);
	float D5 = glm::dot(AB, CP);
	float D6 = glm::dot(AC, CP);
	if (D6 >= 0.f && D5 <= D6)
		return (triangle[2] + Pos);

	float VB = D5 * D2 - D1 * D6;
	if (VB <= 0.f && D2 >= 0.f && D6 <= 0.f)
	{
		float W = D2 / (D2 - D6);
		return Pos + triangle[0] + W * AC;
	}

	float VA = D3 * D6 - D5 * D4;
	if (VA <= 0.f && (D4 - D3) >= 0.f && (D5 - D6) >= 0.f)
	{
		float W = (D4 - D3) / ((D4 - D3) + (D5 - D6));
		return Pos + triangle[1] + W * (triangle[2] - triangle[1]);
	}
	float Denom = 1.0f / (VA + VB + VC);
	float V = VB * Denom;
	float W = VC * Denom;

	return (Pos + triangle[0] + V * AB + W * AC);
}

glm::vec3 MATH::ClosestPoint_Pnt(std::vector<glm::vec3> Seg, glm::vec3 Pos, glm::vec3 Point)
{
	glm::vec3 AB = Seg[1] - Seg[0];

	float s = glm::dot(AB, AB);
	float d = glm::dot((Point - (Pos + Seg[0])), AB) / s;

	d = glm::clamp(d, 0.f, 1.f);

	return Pos + Seg[0] + d * AB;
}

float MATH::Distance_Pnt(std::vector<glm::vec3> Seg, glm::vec3 Pos, glm::vec3 Point)
{
	glm::vec3 P1 = MATH::ClosestPoint_Pnt(Seg, Pos, Point);

	return glm::distance(P1,Point);
}

float MATH::Distance_Seg(std::vector<glm::vec3> Seg0, glm::vec3 Pos, std::vector<glm::vec3> Seg1)
{
	float EPSILON = 0.00000001;

	glm::vec3 D1 = Seg0[1] - Seg0[0];
	glm::vec3 D2 = Seg1[1] - Seg1[0];
	glm::vec3 R = Pos + Seg0[0] - Seg1[0];

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
		C1 = Seg0[1]  + Pos;
		C2 = Seg1[1];
		return glm::distance(C1, C2);
	}

	if (A <= EPSILON)
	{
		S = 0.f;
		T = F / E;
		T = glm::clamp(T, 0.f, 1.f);
	}
	else
	{
		float C = glm::dot(D1, R);
		if (E <= EPSILON)
		{
			T = 0.f;
			S = glm::clamp(-C / A, 0.f, 1.f);
		}
		else
		{
			float B = glm::dot(D1, D2);
			float Denom = A * E - B * B;
			if (Denom != 0.f)
			{
				S = glm::clamp((B * F - C * E), 0.f, 1.f);
			}
			else
			{
				S = 0.f;
			}
			T = (B * S + F) / E;

			if (T < 0.f)
			{
				T = 0.f;
				S = glm::clamp(-C / A, 0.f, 1.f);
			}
			else if (T > 1.f)
			{
				T = 1.f;
				S = glm::clamp((B - C) / A, 0.f, 1.f);
			}
		}
	}

	C1 = Pos + Seg0[1] + D1 * S;
	C2 = Seg1[1] + D2 * T;

	return glm::distance(C1,C2);
}

float MATH::Distance_Seg(std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1)
{
	float EPSILON = 0.00000001;

	glm::vec3 D1 = Seg0[0] - Seg0[1];
	glm::vec3 D2 = Seg1[0] - Seg1[1];
	glm::vec3 R = Seg0[0] - Seg1[0];

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
		C1 = Seg0[1];
		C2 = Seg1[1];
		return glm::distance(C1,C2);
	}

	if (A <= EPSILON)
	{
		S = 0.f;
		T = F / E;
		T = glm::clamp(T, 0.f, 1.f);
	}
	else
	{
		float C = glm::dot(D1, R);
		if (E <= EPSILON)
		{
			T = 0.f;
			S = glm::clamp(-C / A, 0.f, 1.f);
		}
		else
		{
			float B = glm::dot(D1, D2);
			float Denom = A * E - B * B;
			if (Denom != 0.f)
			{
				S = glm::clamp((B * F - C * E), 0.f, 1.f);
			}
			else
			{
				S = 0.f;
			}
			T = (B * S + F) / E;

			if (T < 0.f)
			{
				T = 0.f;
				S = glm::clamp(-C / A, 0.f, 1.f);
			}
			else if (T > 1.f)
			{
				T = 1.f;
				S = glm::clamp((B - C) / A, 0.f, 1.f);
			}
		}
	}

	C1 = Seg0[1] + D1 * S;
	C2 = Seg1[1] + D2 * T;

	return glm::distance(C1,C2);
}

float MATH::Distance_Tr_Pnt(std::vector<glm::vec3> Tr, glm::vec3 Pos, glm::vec3 Pnt)
{
	glm::vec3 Dis = MATH::ClosestPoint_Seg(Tr, Pos, Pnt);
	return glm::distance(Dis,Pnt);
}

bool MATH::ProjColl(std::vector<glm::vec3> Seg, std::vector<glm::vec3> Sh_Vert0, std::vector<glm::vec3> Sh_Vert1)
{
	//Max Min of Shape 0
	glm::vec3 Max0;
	glm::vec3 Min0;
	//Max Min of Shape 1
	glm::vec3 Max1;
	glm::vec3 Min1;

	glm::vec3 AB = Seg[1] - Seg[0];
	int Count = 0;
	for (auto& jj : Sh_Vert0)
	{
		if (Count == 0)
		{
			Max0 = jj;
			Min0 = jj;
			Count++;
		}
		glm::vec3 AJJ = jj - Seg[0];
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Seg[0] + Num / Denom * AB;
		MATH::SetMaxMins(Max0, Min0, TempPos);
	}
	Count = 0;
	for (auto& jj : Sh_Vert1)
	{
		if (Count == 0)
		{
			Max1 = jj;
			Min1 = jj;
			Count++;
		}
		glm::vec3 AJJ = jj - Seg[0];
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Seg[0] + Num / Denom * AB;
		MATH::SetMaxMins(Max1, Min1, TempPos);
	}
	float SegDis0 = glm::distance(Max0, Min0);
	float SegDis1 = glm::distance(Max1, Min1);
	float MaxMin0 = glm::distance(Max0, Min1);
	float MaxMin1 = glm::distance(Max1, Min0);

	float TotalLength = SegDis0 + SegDis1;
	float MaxL = glm::max(MaxMin0, MaxMin1);

	return MaxL > TotalLength;
}

bool MATH::ProjColl(glm::vec3 Normal, std::vector<glm::vec3> Sh_Vert0, std::vector<glm::vec3> Sh_Vert1)
{
	//Max Min of Shape 0
	glm::vec3 Max0;
	glm::vec3 Min0;
	//Max Min of Shape 1
	glm::vec3 Max1;
	glm::vec3 Min1;

	glm::vec3 AB = Normal;
	int Count = 0;
	for (auto& jj : Sh_Vert0)
	{
		if (Count == 0)
		{
			Max0 = jj;
			Min0 = jj;
			Count++;
		}
		glm::vec3 AJJ = jj;
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Num / Denom * AB;
		MATH::SetMaxMins(Max0, Min0, TempPos);
	}
	Count = 0;
	for (auto& jj : Sh_Vert1)
	{
		if (Count == 0)
		{
			Max1 = jj;
			Min1 = jj;
			Count++;
		}
		glm::vec3 AJJ = jj;
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Num / Denom * AB;
		MATH::SetMaxMins(Max1, Min1, TempPos);
	}
	float SegDis0 = glm::distance(Max0, Min0);
	float SegDis1 = glm::distance(Max1, Min1);
	float MaxMin0 = glm::distance(Max0, Min1);
	float MaxMin1 = glm::distance(Max1, Min0);

	float TotalLength = SegDis0 + SegDis1;
	float MaxL = glm::max(MaxMin0, MaxMin1);

	return MaxL > TotalLength;
}

void MATH::SetMaxMins(glm::vec3& Max, glm::vec3& Min, glm::vec3 NewVal)
{
	//Check if Greater in Max
	if (Max.x < NewVal.x)
	{
		Max.x = NewVal.x;
	}
	if (Max.y < NewVal.y)
	{
		Max.y = NewVal.y;
	}
	if (Max.z < NewVal.z)
	{
		Max.z = NewVal.z;
		return;
	}
	//Check if Greater in Min
	if (Min.x > NewVal.x)
	{
		Min.x = NewVal.x;
	}
	if (Min.y > NewVal.y)
	{
		Min.y = NewVal.y;
	}
	if (Min.z > NewVal.z)
	{
		Min.z = NewVal.z;
	}
}

