#include "OBB.h"
using namespace CoatlPhysicsEngine;
OBB::OBB(glm::vec3 Pos, float DimXYZ)
	:ColShapes(Pos)
{
	this->QuatAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->Ex.x = DimXYZ;
	this->Ex.y = DimXYZ;
	this->Ex.z = DimXYZ;
}

OBB::OBB(glm::vec3 Pos, float DimX, float DimYZ)
	:ColShapes(Pos)
{
	this->QuatAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->Ex.x = DimX;
	this->Ex.y = DimYZ;
	this->Ex.z = DimYZ;
}

OBB::OBB(glm::vec3 Pos, float DimX, float DimY, float DimZ)
	:ColShapes(Pos)
{
	this->QuatAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->Ex.x = DimX;
	this->Ex.y = DimY;
	this->Ex.z = DimZ;
}

OBB::OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimXYZ)
	:ColShapes(Pos)
{
	float InitAng = InitAngle / 180.f * glm::pi<float>();
	this->QuatAngle = glm::angleAxis(InitAng,InitUnitVec);
	this->Ex.x = DimXYZ ;
	this->Ex.y = DimXYZ;
	this->Ex.z = DimXYZ;
}

OBB::OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimX, float DimYZ)
	: ColShapes(Pos)
{
	float InitAng = InitAngle / 180.f * glm::pi<float>();
	this->QuatAngle = glm::angleAxis(InitAng, InitUnitVec);
	this->Ex.x = DimX ;
	this->Ex.y = DimYZ;
	this->Ex.z = DimYZ;
}

OBB::OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimX, float DimY, float DimZ)
	: ColShapes(Pos)
{
	float InitAng = InitAngle / 180.f * glm::pi<float>();
	this->QuatAngle = glm::angleAxis(InitAng, InitUnitVec);
	this->Ex.x = DimX;
	this->Ex.y = DimY;
	this->Ex.z = DimZ;
}

OBB::~OBB()
{
}

std::vector<glm::vec3> OBB::GetSegments()
{
	glm::mat4 R = glm::mat4_cast(this->QuatAngle);
	glm::mat4 T = glm::translate(glm::mat4(1.f), Pos);

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
		glm::vec4 Set = glm::vec4(jj, 1.f);
		jj = T * R * Set;
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
	std::vector<glm::vec3> Pnts = this->GetSegments();
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

glm::vec3 OBB::EPA_Support(glm::vec3 Dir)
{
	return this->Support(Dir);
}

std::vector<glm::vec3> OBB::GetVertices()
{
	return this->GetSegments();
}

std::vector<glm::vec3> OBB::GetNormals()
{
	std::vector<glm::vec3> T = this->GetSegments();
	std::vector<glm::vec3> OBB_N = { glm::normalize(T[1] - T[0]),
		glm::normalize(T[3] - T[0]) ,
		glm::normalize(T[4] - T[0]) };
	return OBB_N;
}

glm::mat3 OBB::GetInertia(float Mass)
{
	glm::vec3 Tex = 2.f * Ex;
	float LH2 = (Tex.x * Tex.x + Tex.z * Tex.z) / 12.f;
	float HW2 = (Tex.y * Tex.y + Tex.x * Tex.x) / 12.f;
	float LW2 = (Tex.z * Tex.z + Tex.y * Tex.y) / 12.f;
	float LW = 0.f;
	float LH = 0.f;
	float HW = 0.f;
	glm::vec3 Col0 = glm::vec3(LH2,LW,HW);
	glm::vec3 Col1 = glm::vec3(LW,HW2,LH);
	glm::vec3 Col2 = glm::vec3(HW,LH,LW2);
	return Mass * glm::mat3(Col0, Col1, Col2);
}
