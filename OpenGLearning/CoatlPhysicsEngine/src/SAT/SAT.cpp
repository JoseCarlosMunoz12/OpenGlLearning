#include "SAT.h"
using namespace CoatlPhysicsEngine;
float SAT::ProjPen(glm::vec3& Normal, std::vector<glm::vec3> Sh_Vert0, std::vector<glm::vec3> Sh_Vert1)
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

float SAT::SAT_Algo(std::vector<glm::vec3> Norm0, std::vector<glm::vec3> Norm1,
	std::vector<glm::vec3> Pnts0, std::vector<glm::vec3> Pnts1, glm::vec3& Norm)
{
	glm::vec3 NormF;
	if (Norm0.size() != 0)
	{
		NormF = Norm0[0];
	}
	else
	{
		NormF = Norm1[0];
	}
	float Penetration = ProjPen(NormF, Pnts0, Pnts1);
	for (auto& jj : Norm0)
	{
		float Temp = ProjPen(jj, Pnts0, Pnts1);
		if (Temp < Penetration)
		{
			NormF = jj;
			Penetration = Temp;
		}
	}
	for (auto& jj : Norm1)
	{
		float Temp = ProjPen(jj, Pnts0, Pnts1);
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
				float Temp = ProjPen(N, Pnts0, Pnts1);
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

float SAT::GetPenetrationContacts(std::shared_ptr<ColShapes> Bod0, std::shared_ptr<ColShapes> Bod1, glm::vec3& Norm)
{
	std::vector<glm::vec3> Vertex0 = Bod0->GetVertices();
	std::vector<glm::vec3> Vertex1 = Bod1->GetVertices();
	std::vector<glm::vec3> Normals0 = Bod0->GetNormals();
	std::vector<glm::vec3> Normals1 = Bod1->GetNormals();
	return SAT_Algo(Normals0, Normals1, Vertex0, Vertex1, Norm);
}

bool SAT::SAT_Check(std::shared_ptr<ColShapes> Bod0, std::shared_ptr<ColShapes> Bod1)
{
	std::vector<glm::vec3> Vertex0 = Bod0->GetVertices();
	std::vector<glm::vec3> Vertex1 = Bod1->GetVertices();
	std::vector<glm::vec3> Normals0 = Bod0->GetNormals();
	std::vector<glm::vec3> Normals1 = Bod1->GetNormals();
	return MATH::SATColCheck(Normals0, Normals1, Vertex0, Vertex1);
}

std::vector<std::shared_ptr<Contacts>> SAT::SAT_CreateContacts(std::shared_ptr<ColShapes> Bod0, std::shared_ptr<ColShapes> Bod1,
	glm::vec3 Norm, float Pen)
{
	std::vector<std::shared_ptr<Contacts>> Temp;
	std::vector<glm::vec3> Obj0_seg = Bod0->GetVertices();
	std::vector<glm::vec3> Obj1_seg = Bod1->GetVertices();
	std::vector<glm::vec3> Obj1_Norm = Bod1->GetNormals();
	MATH::SAT_Point_Cul(Norm, Obj0_seg, Obj1_seg);
	for (auto& ii : Obj1_Norm)
	{
		glm::vec3 Dot = glm::cross(ii, Norm);
		if (Dot != glm::vec3(0.f))
			MATH::SAT_Clip(ii, Obj0_seg, Obj1_seg);
	}
	int Count = 0;
	for (auto& jj : Obj0_seg)
	{
		std::shared_ptr<Contacts> Cont = std::make_shared<Contacts>();
		Cont->Normal = Norm;
		Cont->Penetration = Pen;
		Cont->ContactPoint = (jj + Obj0_seg[Count]) / 2.f;
		Temp.push_back(Cont);
		Count++;
	}
	return Temp;
}
