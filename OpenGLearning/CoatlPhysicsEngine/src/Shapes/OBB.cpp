#include "OBB.h"
using namespace CoatlPhysicsEngine;
std::vector<glm::vec3> CoatlPhysicsEngine::OBB::GetSegOffset(float Ext)
{
	glm::mat4 RotMat = glm::mat4_cast(this->QuatAngle);
	glm::vec3 Exts = Ex;
	Exts.x += Ext;
	Exts.y += Ext;
	Exts.z += Ext;
	glm::vec3 Positions[] =
	{
		//Bottom Plane
		glm::vec3(Exts.x,-Exts.y,-Exts.z), glm::vec3(Exts.x, Exts.y,-Exts.z),
		glm::vec3(-Exts.x, Exts.y,-Exts.z), glm::vec3(-Exts.x,-Exts.y,-Exts.z),
		//Top Plane 
		glm::vec3(Exts.x,-Exts.y, Exts.z), glm::vec3(Exts.x, Exts.y, Exts.z),
		glm::vec3(-Exts.x, Exts.y, Exts.z), glm::vec3(-Exts.x,-Exts.y, Exts.z)
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
OBB::OBB(glm::vec3 Pos, float DimXYZ)
	:ColShapes(Pos)
{
	this->QuatAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->Ex.x = DimXYZ / 2;
	this->Ex.y = DimXYZ / 2;
	this->Ex.z = DimXYZ / 2;
}

OBB::OBB(glm::vec3 Pos, float DimX, float DimYZ)
	:ColShapes(Pos)
{
	this->QuatAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->Ex.x = DimX/2;
	this->Ex.y = DimYZ/2;
	this->Ex.z = DimYZ/2;
}

OBB::OBB(glm::vec3 Pos, float DimX, float DimY, float DimZ)
	:ColShapes(Pos)
{
	this->QuatAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->Ex.x = DimX/2;
	this->Ex.y = DimY/2;
	this->Ex.z = DimZ/2;
}

OBB::OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimXYZ)
	:ColShapes(Pos)
{
	float InitAng = InitAngle / 180.f * glm::pi<float>();
	this->QuatAngle = glm::angleAxis(InitAng,InitUnitVec);
	this->Ex.x = DimXYZ / 2;
	this->Ex.y = DimXYZ / 2;
	this->Ex.z = DimXYZ / 2;
}

OBB::OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimX, float DimYZ)
	: ColShapes(Pos)
{
	float InitAng = InitAngle / 180.f * glm::pi<float>();
	this->QuatAngle = glm::angleAxis(InitAng, InitUnitVec);
	this->Ex.x = DimX / 2;
	this->Ex.y = DimYZ / 2;
	this->Ex.z = DimYZ / 2;
}

OBB::OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimX, float DimY, float DimZ)
	: ColShapes(Pos)
{
	float InitAng = InitAngle / 180.f * glm::pi<float>();
	this->QuatAngle = glm::angleAxis(InitAng, InitUnitVec);
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
		//Bottom Plane
		glm::vec3(Ex.x,-Ex.y,-Ex.z), glm::vec3(Ex.x, Ex.y,-Ex.z),
		glm::vec3(-Ex.x, Ex.y,-Ex.z), glm::vec3(-Ex.x,-Ex.y,-Ex.z),
		//Top Plane 
		glm::vec3(Ex.x,-Ex.y, Ex.z), glm::vec3(Ex.x, Ex.y, Ex.z),
		glm::vec3(-Ex.x, Ex.y, Ex.z), glm::vec3(-Ex.x,-Ex.y, Ex.z)
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

glm::vec3 OBB::Support(glm::vec3 Dir)
{
	std::vector<glm::vec3> Pnts = this->GetSegOffset(0.00001f);
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
