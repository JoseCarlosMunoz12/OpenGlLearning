#include "Capsule.h"
using namespace CoatlPhysicsEngine;

Capsule::Capsule(glm::vec3 Pos, float InitRadius, float InitLength)
	:ColShapes(Pos),UnitVec(glm::vec3(0.f,0.f,1.f))
{
	this->Radius = InitRadius;
	this->BPos = glm::vec3(0.f,0.f,InitLength/2.f);
	this->APos = -glm::vec3(0.f, 0.f, InitLength/2.f);
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
	return MATH::Distance_Pnt({ APos,BPos }, Pos, Point);
}

float Capsule::Distance(std::vector<glm::vec3> Segment)
{
	return MATH::Distance_Seg({APos,BPos},Pos,Segment);
}

std::vector<glm::vec3> Capsule::GetSegment()
{
	glm::mat4 RotMat = glm::mat4_cast(this->QuatAngle);
	std::vector<glm::vec3> Verx;
	{
		glm::vec4 Set = glm::vec4(this->APos.x,
			this->APos.y, this->APos.z, 
			0);
		Set = Set * RotMat;
		Set = Set ;
		Verx.push_back(glm::vec3(Set.x, Set.y, Set.z) + this->Pos);
	}
	{
		glm::vec4 Set = glm::vec4(this->BPos.x,
			this->BPos.y, this->BPos.z,
			0);
		Set = Set * RotMat;
		Verx.push_back(glm::vec3(Set.x, Set.y, Set.z) + this->Pos);
	}
	return Verx;
}

glm::vec3 Capsule::GetUnitVec()
{
	return this->UnitVec;
}


void Capsule::SetVec(glm::vec3 NewVec)
{
	this->UnitVec = glm::normalize(NewVec);
}

void Capsule::SetQuat(glm::quat NewQuat)
{
	this->QuatAngle = NewQuat;
}

glm::vec3 Capsule::Support(glm::vec3 Dir)
{
	std::vector<glm::vec3> Pnts = this->GetSegment();
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
	glm::vec3 Norm = glm::normalize(Dir);
	return MaxPnt + (this->Radius + 0.001f) * Norm;
}

glm::vec3 CoatlPhysicsEngine::Capsule::EPA_Support(glm::vec3 Dir)
{
	std::vector<glm::vec3> Pnts = this->GetSegment();
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
	glm::vec3 Norm = glm::normalize(Dir);
	return MaxPnt;
}
