#include "Capsule.h"
using namespace CoatlPhysicsEngine;

Capsule::Capsule(glm::vec3 Pos, float InitRadius, float InitLength)
	:ColShapes(Pos)
{
	this->Radius = InitRadius;
	this->BPos = glm::vec3(0.f,0.f,InitLength/2);
	this->APos = -glm::vec3(0.f, 0.f, InitLength / 2);
}

Capsule::~Capsule()
{
}

float Capsule::GetRadius()
{
	return this->Radius;
}

glm::vec3 Capsule::ClosestPoint_P(glm::vec3 Point)
{
	return MATH::ClosestPoint_Pnt({APos,BPos}, Pos, Point);
}

float Capsule::Distance(glm::vec3 Point)
{
	return MATH::Distance_Cap_Pnt({ APos,BPos }, Pos, Point);
}

float Capsule::Distance(std::vector<glm::vec3> Segment)
{
	float EPSILON = 0.00000001;

	glm::vec3 D1 = this->APos - this->BPos;
	glm::vec3 D2 = Segment[0] - Segment[1];
	glm::vec3 R = this->Pos +  this->BPos - Segment[1];

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
		C1 = this->BPos + this->Pos;
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

	C1 = this->Pos + this->BPos + D1 * S;
	C2 = Segment[1] + D2 * T;

	return glm::dot(C1 - C2, C1 - C2);
}

std::vector<glm::vec3> Capsule::GetSegment()
{
	return {this->Pos + this->APos,
		this->Pos + this->BPos};
}
