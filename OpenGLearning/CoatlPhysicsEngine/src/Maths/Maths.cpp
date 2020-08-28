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

glm::vec3 MATH::ClosestPoint_Seg(std::vector<glm::vec3> Seg, glm::vec3 Point)
{
	glm::vec3 AB = Seg[1] - Seg[0];

	float s = glm::dot(AB, AB);
	if (s == 0.f)
		return Seg[0];
	float d = glm::dot((Point - Seg[0]), AB) / s;

	d = glm::clamp(d, 0.f, 1.f);

	return Seg[0] + d * AB;
}

glm::vec3 MATH::ClosestPoint_Pnt(std::vector<glm::vec3> Seg, glm::vec3 Pos, glm::vec3 Point)
{
	glm::vec3 AB = Seg[1] - Seg[0];

	float s = glm::dot(AB, AB);
	if (s == 0)
		return Pos + Seg[0];
	float d = glm::dot((Point - (Pos + Seg[0])), AB) / s;

	d = glm::clamp(d, 0.f, 1.f);

	return Pos + Seg[0] + d * AB;
}

void MATH::ClosestSeg_Seg(std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1, glm::vec3& Pos0, glm::vec3& Pos1)
{
	float EPSILON = 0.00000001;

	glm::vec3 D1 = Seg0[1] - Seg0[0];
	glm::vec3 D2 = Seg1[1] - Seg1[0];
	glm::vec3 R = Seg0[0] - Seg1[0];

	float A = glm::dot(D1, D1);
	float E = glm::dot(D2, D2);
	float F = glm::dot(D2, R);

	float S, T;
	if (A <= EPSILON && E <= EPSILON)
	{
		S = 0.f;
		T = 0.f;
		Pos0 = Seg0[0];
		Pos1 = Seg1[0];
		return;
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
				S = glm::clamp((B * F - C * E) / Denom, 0.f, 1.f);
			}
			else
			{
				S = 0.f;
			}
			T = ((B * S) + F) / E;

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

	Pos0 = Seg0[0] + D1 * S;
	Pos1 = Seg1[0] + D2 * T;
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
		C1 = Seg0[0]  + Pos;
		C2 = Seg1[0];
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
				S = glm::clamp((B * F - C * E) / Denom, 0.f, 1.f);
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

	C1 = Pos + Seg0[0] + D1 * S;
	C2 = Seg1[0] + D2 * T;

	return glm::distance(C1,C2);
}

float MATH::Distance_Seg(std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1)
{
	float EPSILON = 0.00000001;

	glm::vec3 D1 = Seg0[1] - Seg0[0];
	glm::vec3 D2 = Seg1[1] - Seg1[0];
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
		C1 = Seg0[0];
		C2 = Seg1[0];
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

	C1 = Seg0[0] + D1 * S;
	C2 = Seg1[0] + D2 * T;

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

bool MATH::SATColCheck(std::vector<glm::vec3> Norm0, std::vector<glm::vec3> Norm1, std::vector<glm::vec3> Pnts0, std::vector<glm::vec3> Pnts1)
{
	for (auto& jj : Norm0)
	{
		if (MATH::ProjColl(jj, Pnts0, Pnts1))
		{
			return false;
		}
	}
	for (auto& jj : Norm1)
	{
		if (MATH::ProjColl(jj, Pnts0, Pnts1))
		{
			return false;
		}
	}
	for (auto& ii : Norm0)
	{
		for (auto& jj : Norm1)
		{
			glm::vec3 N = glm::cross(ii, jj);
			if (!(N.x == 0 && N.y == 0 && N.z == 0))
			{
				if (MATH::ProjColl(N, Pnts0, Pnts1))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void MATH::SetMaxMins(glm::vec3& Max, glm::vec3& Min, glm::vec3 NewVal)
{
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
		Min = NewVal;
	}
}

glm::vec3 MATH::SetMax(glm::vec3 Max, glm::vec3 NewVal)
{
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
	}
	return Max;
}

glm::vec3 MATH::SetMin(glm::vec3 Min, glm::vec3 NewVal)
{
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
	return Min;
}

void MATH::SetRejDir(glm::vec3& Max, glm::vec3& Min, glm::vec3 NewVal,
	glm::vec3& RejMax, glm::vec3& RejMin, glm::vec3 OgSeg)
{
	//Check if Greater in Max
	if (Max.x < NewVal.x && Max.y < NewVal.y && Max.z < NewVal.z)
	{
		Max = NewVal;
		RejMax = OgSeg - NewVal;
		return;
	}
	//Check if Greater in Min
	if (Min.x > NewVal.x && Min.y > NewVal.y && Min.z > NewVal.z)
	{
		Min = NewVal;
		RejMin = OgSeg - NewVal;
	}
}

float MATH::ProjPen(glm::vec3& Normal, std::vector<glm::vec3> Sh_Vert0, std::vector<glm::vec3> Sh_Vert1, float R)
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
		glm::vec3 AJJ = jj;
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Num / Denom * AB;
		if (Count == 0)
		{
			Max0 = TempPos;
			Min0 = TempPos;
			Count++;
		}
		MATH::SetMaxMins(Max0, Min0, TempPos);
	}
	Count = 0;
	for (auto& jj : Sh_Vert1)
	{
		glm::vec3 AJJ = jj;
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Num / Denom * AB;
		if (Count == 0)
		{
			Max1 = TempPos;
			Min1 = TempPos;
			Count++;
		}
		MATH::SetMaxMins(Max1, Min1, TempPos);
	}
	float MaxMin0 = glm::distance(Max0, Min1);
	float MaxMin1 = glm::distance(Max1, Min0);
	float M0 = glm::distance(Max0, Min0);
	float M1 = glm::distance(Max1, Min1);
	float MinL = glm::min(MaxMin0, MaxMin1);
	if (MinL == MaxMin1)
	{
		Normal = -Normal;
	}
	return  MinL;
}

glm::vec3 MATH::Proj(glm::vec3 Norm, glm::vec3 Pnt)
{
	float Num = glm::dot(Norm, Pnt);
	float Denom = glm::dot(Norm, Norm);
	return Num / Denom * Norm;
}

float MATH::SATContact(std::vector<glm::vec3> Norm0, std::vector<glm::vec3> Norm1,
	std::vector<glm::vec3> Pnts0, std::vector<glm::vec3> Pnts1,
	glm::vec3 &Norm,float R)
{
	glm::vec3 NormF = Norm0[0];
	float Penetration = ProjPen(NormF,Pnts0,Pnts1,R);
	for (auto& jj : Norm0)
	{
		float Temp = ProjPen(jj, Pnts0, Pnts1,R);
		if (Temp < Penetration)
		{
			NormF = jj;
			Penetration = Temp;
		}
	}
	for (auto& jj : Norm1)
	{
		float Temp = ProjPen(jj, Pnts0, Pnts1,R);
		if (Temp < Penetration)
		{
			NormF = jj;
			Penetration = Temp;
		}
	}
	for (auto& ii : Norm0)
	{
		for (auto& jj : Norm1)
		{
			glm::vec3 N = glm::cross(ii, jj);
			if (!(N.x == 0 && N.y == 0 && N.z == 0))
			{
				float Temp = ProjPen(jj, Pnts0, Pnts1,R);
				if (Temp < Penetration)
				{
					NormF = jj;
					Penetration = Temp;
				}
			}
		}
	}
	Norm = NormF;
	return Penetration;
}

std::vector<glm::vec3> MATH::SAT_Points(glm::vec3 Norm, std::vector<glm::vec3> Vert0, std::vector<glm::vec3> Vert1)
{
	//Max Min of Shape 0
	glm::vec3 Max0;
	glm::vec3 RejMax0;
	glm::vec3 Min0;
	glm::vec3 RejMin0;
	//Max Min of Shape 1
	glm::vec3 Max1;
	glm::vec3 RejMax1;
	glm::vec3 Min1;
	glm::vec3 RejMin1;

	glm::vec3 AB = Norm;
	int Count = 0;
	for (auto& jj : Vert0)
	{
		glm::vec3 AJJ = jj;
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Num / Denom * AB;
		if (Count == 0)
		{
			Max0 = TempPos;
			Min0 = TempPos;
			Count++;
		}
		MATH::SetMaxMins(Max0, Min0, TempPos);
	}
	Count = 0;
	for (auto& jj : Vert1)
	{
		glm::vec3 AJJ = jj;
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Num / Denom * AB;
		if (Count == 0)
		{
			Max1 = TempPos;
			Min1 = TempPos;
			Count++;
		}
		MATH::SetMaxMins(Max1, Min1, TempPos);
	}
	float MaxMin0 = glm::distance(Max0, Min1);
	float MaxMin1 = glm::distance(Max1, Min0);
	if (MaxMin0 < MaxMin1)
		return { Min1,Max0};
	return {Min0,Max1 };
}

glm::vec3 MATH::SAT_Point(glm::vec3 Norm, std::vector<glm::vec3> Vert0, std::vector<glm::vec3> Vert1)
{
	//Max Min of Shape 0
	glm::vec3 Max0;
	glm::vec3 RejMax0;
	glm::vec3 Min0;
	glm::vec3 RejMin0;
	//Max Min of Shape 1
	glm::vec3 Max1;
	glm::vec3 RejMax1;
	glm::vec3 Min1;
	glm::vec3 RejMin1;

	glm::vec3 AB = Norm;
	int Count = 0;
	for (auto& jj : Vert0)
	{
		glm::vec3 AJJ = jj;
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Num / Denom * AB;
		if (Count == 0)
		{
			Max0 = TempPos;
			Min0 = TempPos;
			RejMax0 = Vert0[0] - Max0;
			RejMin0 = Vert0[0] - Min0;
			Count++;
		}
		MATH::SetRejDir(Max0, Min0, TempPos,RejMax0, RejMin0,Vert0[0]);
	}
	Count = 0;
	for (auto& jj : Vert1)
	{
		glm::vec3 AJJ = jj;
		float Num = glm::dot(AJJ, AB);
		float Denom = glm::dot(AB, AB);
		glm::vec3 TempPos = Num / Denom * AB;
		if (Count == 0)
		{
			Max1 = TempPos;
			Min1 = TempPos;
			Count++;
		}
		MATH::SetMaxMins(Max1, Min1, TempPos);
	}
	float MaxMin0 = glm::distance(Max0, Min1);
	float MaxMin1 = glm::distance(Max1, Min0);
	if (MaxMin0 > MaxMin1)
		return Max0 + RejMax0;
	return Min0 + RejMin0;
}

void MATH::SAT_Point_Cul(glm::vec3 Norm, std::vector<glm::vec3>& Vert0, std::vector<glm::vec3>& Vert1)
{
	std::vector<glm::vec3> InitV0 = Vert0;
	std::vector<glm::vec3> InitV1 = Vert1;
	//Max Min of Shape 0
	glm::vec3 Max0;
	std::vector<int> MaxID0;
	glm::vec3 Min0;
	std::vector<int> MinID0;
	//Max Min of Shape 1
	glm::vec3 Max1;
	std::vector<int> MaxID1;
	glm::vec3 Min1;
	std::vector<int> MinID1;
	//CalcValues
	int Count = 0;
	for (auto& ii : InitV0)
	{
		float Num = glm::dot(ii, Norm);
		float Denom = glm::dot(Norm, Norm);
		glm::vec3 TempPos = Num / Denom * Norm;
		if (Count != 0)
		{
			MATH::SAT_MAXMIN_Cul(Max0, Min0, TempPos, Count, MaxID0, MinID0);
		}
		else
		{
			Max0 = TempPos;
			Min0 = TempPos;
			MaxID0.push_back(Count);
			MinID0.push_back(Count);
			MATH::SetMaxMins(Max0, Min0, TempPos);
		}
		Count++;
	}
	Count = 0;
	for (auto& ii : InitV1)
	{
		float Num = glm::dot(ii, Norm);
		float Denom = glm::dot(Norm, Norm);
		glm::vec3 TempPos = Num / Denom * Norm;
		if (Count != 0)
		{
			MATH::SAT_MAXMIN_Cul(Max1, Min1, TempPos, Count, MaxID1, MinID1);
		}
		else
		{
			Max1 = TempPos;
			Min1 = TempPos;
			MaxID1.push_back(Count);
			MinID1.push_back(Count);
			MATH::SetMaxMins(Max1, Min1, TempPos);
		}
		Count++;
	}
	//Find min Point
	float MaxMin0 = glm::distance(Max0, Min1);
	float MaxMin1 = glm::distance(Max1, Min0);
	//clear points
	Vert0.clear();
	Vert1.clear();
	if (MaxMin0 < MaxMin1)
	{
		for (auto& jj : MaxID0)
		{
			Vert0.push_back(InitV0[jj]);
		}
		for (auto& jj : MinID1)
		{
			Vert1.push_back(InitV1[jj]);
		}
		return;
	}
	for (auto& jj : MinID0)
	{
		Vert0.push_back(InitV0[jj]);
	}
	for (auto& jj : MaxID1)
	{
		Vert1.push_back(InitV1[jj]);
	}
}

void MATH::SAT_MAXMIN_Cul(glm::vec3& Max, glm::vec3& Min, glm::vec3 NewVal, int ID, std::vector<int>& ID0, std::vector<int>& ID1)
{

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
		ID0.clear();
		ID0.push_back(ID);
	}
	if (Max == NewVal)
	{
		if (std::find(ID0.begin(), ID0.end(), ID) == ID0.end())
			ID0.push_back(ID);
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
		ID1.clear();
		ID1.push_back(ID);
		return;
	}
	if(Min == NewVal)
	{
		if (std::find(ID1.begin(),ID1.end(),ID) == ID1.end())
			ID1.push_back(ID);
	}
}

void MATH::SAT_Clip(glm::vec3 Norm, std::vector<glm::vec3>& Vert0, std::vector<glm::vec3>& Vert1)
{
	if (Vert0.size() == 0)
	{
		float s = 45;
	}
	glm::vec3 Vec0 = MATH::MaxDot(Vert0, Norm);
	glm::vec3 Vec1 = MATH::MaxDot(Vert1, Norm);
	glm::vec3 Vec4 = MATH::MaxDot({Vec0, Vec1}, -Norm);
	MATH::SAT_VecChange(-Norm, Vec4, Vert0);
	MATH::SAT_VecChange(-Norm, Vec4, Vert1);
	glm::vec3 Vec2 = MATH::MaxDot(Vert0, -Norm);
	glm::vec3 Vec3 = MATH::MaxDot(Vert1, -Norm);
	glm::vec3 Vec5 = MATH::MaxDot({ Vec2, Vec3 }, Norm);
	MATH::SAT_VecChange(Norm, Vec5, Vert0);
	MATH::SAT_VecChange(Norm, Vec5, Vert1);
}

void MATH::SAT_VecChange(glm::vec3 Norm, glm::vec3 Pnt, std::vector<glm::vec3>& Vert)
{
	std::vector<glm::vec3> NewPnts;
	glm::vec3 PntRel = MATH::Proj(Norm, Pnt);
	for (auto& jj : Vert)
	{
		glm::vec3 PntLoc = MATH::Proj(Norm, jj);
		glm::vec3 relNorm = MATH::Normalize(PntLoc - PntRel);
		if (glm::dot(Norm, relNorm) < 0.f)
		{
			float dis = glm::distance(PntLoc, PntRel);
			jj = jj + dis * Norm;
		}
		if (std::find(NewPnts.begin(), NewPnts.end(), jj) == NewPnts.end())
		{
			NewPnts.push_back(jj);
		}
	}
	Vert = NewPnts;
}

glm::vec3 MATH::MaxDot(std::vector<glm::vec3> Pnts, glm::vec3 Dir)
{
	float S = glm::dot(Pnts[0], Dir);
	glm::vec3 MaxPnt = Pnts[0];
	int Size = Pnts.size();
	for (int ii = 1; ii < Size; ii++)
	{
		float T = glm::dot(Pnts[ii], Dir);
		if (T > S)
		{
			S = T;
			MaxPnt = Pnts[ii];
		}
	}
	return MaxPnt;
}

glm::vec3 MATH::Support(std::vector<glm::vec3> Pnt0, std::vector<glm::vec3> Pnt1, glm::vec3 Dir)
{
	return MaxDot(Pnt0,Dir) -  MaxDot(Pnt1,-Dir);
}

bool MATH::DoSimplex(std::vector<glm::vec3>& Simplex, glm::vec3& Dir)
{
	int Length = Simplex.size();
	if (Length == 2)
	{
		glm::vec3 B = Simplex[0];
		glm::vec3 A = Simplex[1];
		glm::vec3 A0 = -A;
		glm::vec3 AB = B - A;
		if (glm::dot(AB, A0) >= 0)
		{
			glm::vec3 Temp = glm::cross(AB,A0);
			Dir = glm::cross(Temp, AB);
			Dir = glm::normalize(Dir);
		}
		else
		{
			Simplex.erase(Simplex.begin());
			Dir = glm::normalize(A0);
		}
	}
	else
	{
		glm::vec3 C = Simplex[0];
		glm::vec3 B = Simplex[1];
		glm::vec3 A = Simplex[2];
		glm::vec3 A0 = -A ;
		glm::vec3 AB = B - A;
		glm::vec3 AC = C - A;
		if (glm::dot(AB, A0) >= 0)
		{
			glm::vec3 Cross = glm::cross(AB, A0);
			Cross = glm::cross(Cross, AB);
			if (glm::dot(Cross, AC) >= 0)
			{
				if (glm::dot(Cross, AB) >= 0)
				{
					return true;
				}
				else
				{
					Simplex.erase(Simplex.begin() + 1);
					Dir = glm::normalize(Cross);
				}
			}
			else
			{
				Simplex.erase(Simplex.begin());
				Dir = glm::normalize(Cross);
			}
		}
		else
		{
			if (glm::dot(AC, A0) >= 0)
			{
				glm::vec3 Cross = glm::cross(AC, A0);
				Cross = glm::cross(Cross, AC);
				if (glm::dot(AB, Cross) >= 0)
				{
					return true;
				}
				else
				{
					Simplex.erase(Simplex.begin() + 1);
					Dir = glm::normalize(Cross);
				}
			}
			else
			{
				Simplex.erase(Simplex.begin() + 1);
				Simplex.erase(Simplex.begin());
				Dir = glm::normalize(A0);
			}
		}
	}
	return false;
}

bool MATH::GJK_Algorithm(std::vector<glm::vec3> Pnt0, std::vector<glm::vec3> Pnt1)
{
	glm::vec3 A = Support(Pnt0, Pnt1, glm::vec3(0.f, 0.f, 1.f));
	std::vector<glm::vec3> S;
	S.push_back(A);
	glm::vec3 D = -glm::normalize(A);
	while (true)
	{
		A = Support(Pnt0, Pnt1, D);
		if (glm::dot(A, D) < 0)
			return false;
		S.push_back(A);
		if (DoSimplex(S, D))
		{
			return true;
		}
	}
	return false;
}

glm::vec3 MATH::CreateNormal(std::vector<glm::vec3> Seg)
{
	return MATH::Normalize(Seg[1] - Seg[0]);
}

glm::vec3 MATH::Normalize(glm::vec3 Vec)
{
	if (glm::dot(Vec, Vec) != 0.f)
		return MATH::ClampLowest(glm::normalize(Vec),0.0001f);
	return MATH::ClampLowest(Vec, 0.0001f);
}

glm::vec3 MATH::ClampLowest(glm::vec3 Vec, float Limit)
{
	if (glm::abs(Vec.x) < Limit)
		Vec.x = 0.f;
	if (glm::abs(Vec.y) < Limit)
		Vec.y = 0.f;
	if (glm::abs(Vec.z) < Limit)
		Vec.z = 0.f;
	return	Vec;
}

bool MATH::IsSame(glm::vec3 Vec0, glm::vec3 Vec1)
{
	Vec0 = ClampLowest(Vec0, 0.001);
	Vec1 = ClampLowest(Vec1, 0.001);
	glm::vec3 T = Vec0 - Vec1;
	return glm::dot(T, T) < 0.001;
}
