#include "OBB.h"
using namespace CoatlPhysicsEngine;
OBB::OBB(glm::vec3 Pos, float DimXYZ)
	:ColShapes(Pos)
{
	this->Ex.x = DimXYZ / 2;
	this->Ex.y = DimXYZ / 2;
	this->Ex.z = DimXYZ / 2;
}

OBB::OBB(glm::vec3 Pos, float DimX, float DimYZ)
	:ColShapes(Pos)
{
	this->Ex.x = DimX/2;
	this->Ex.y = DimYZ/2;
	this->Ex.z = DimYZ/2;
}

OBB::OBB(glm::vec3 Pos, float DimX, float DimY, float DimZ)
	:ColShapes(Pos)
{
	this->Ex.x = DimX/2;
	this->Ex.y = DimY/2;
	this->Ex.z = DimZ/2;
}

CoatlPhysicsEngine::OBB::OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimXYZ)
	:ColShapes(Pos)
{
	this->Ex.x = DimXYZ / 2;
	this->Ex.y = DimXYZ / 2;
	this->Ex.z = DimXYZ / 2;
}

CoatlPhysicsEngine::OBB::OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimX, float DimYZ)
	: ColShapes(Pos)
{
	this->Ex.x = DimX / 2;
	this->Ex.y = DimYZ / 2;
	this->Ex.z = DimYZ / 2;
}

CoatlPhysicsEngine::OBB::OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimX, float DimY, float DimZ)
	: ColShapes(Pos)
{
	this->Ex.x = DimX / 2;
	this->Ex.y = DimY / 2;
	this->Ex.z = DimZ / 2;
}

OBB::~OBB()
{
}


std::vector<glm::vec3> OBB::GetSegments()
{
	glm::mat4 RotMat = glm::mat4_cast(this->QuatAngle);

	glm::vec3 Positions[] =
	{
		//Top Plane 
		glm::vec3(Ex.x,-Ex.y, Ex.z), glm::vec3(Ex.x, Ex.y, Ex.z),
		glm::vec3(-Ex.x, Ex.y, Ex.z), glm::vec3(-Ex.x,-Ex.y, Ex.z),
		//Bottom Plane
		glm::vec3(Ex.x,-Ex.y,-Ex.z), glm::vec3(Ex.x, Ex.y,-Ex.z),
		glm::vec3(-Ex.x, Ex.y,-Ex.z), glm::vec3(-Ex.x,-Ex.y,-Ex.z)
	};
	std::vector<glm::vec3> Lines(std::begin(Positions), std::end(Positions));
	for (auto& jj : Lines)
	{
		jj = jj + this->Pos;
	}
	for (auto& jj : Lines)
	{
		glm::vec4 Set = glm::vec4(jj.x, jj.y, jj.z, 0.f);
		Set = Set * RotMat;
		jj = glm::vec3(Set.x, Set.y, Set.z);
	}
	return Lines;
}

glm::vec3 OBB::GetLenghts()
{
	return this->Ex;
}

glm::vec3 OBB::GetClosestPoint(glm::vec3 Point)
{
	std::vector<glm::vec3> Segs = this->GetSegments();
	glm::vec3 ClsPoints;
	std::vector<int> Ind = {0,1,1,2,2,3,3,0,
		4,5,5,6,6,7,7,4,
		0,4,1,5,2,6,3,7 };
	float Min;
	int Count = 0;
	for (int jj = 0; jj < 6; jj++)
	{
		int Val = jj * 2;
		glm::vec3 TempVec = MATH::ClosestPoint_Seg({ Segs[Ind[Val]], Segs[Ind[Val + 1]]}, Point);
		float Temp = glm::distance(Point, TempVec);
		if (Count > 0)
		{
			if (Min >Temp)
			{
				Min = Temp;
				ClsPoints = TempVec;
			}
		}
		else
		{
			Min = Temp;
			ClsPoints = TempVec;
		}
		Count++;
	}
	return ClsPoints;
}

void OBB::SetQuat(glm::quat NewQuat)
{
	this->QuatAngle = NewQuat;
}
