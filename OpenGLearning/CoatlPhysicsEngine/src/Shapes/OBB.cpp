#include "OBB.h"
using namespace CoatlPhysicsEngine;
OBB::OBB(glm::vec3 Pos, float DimXYZ)
	:ColShapes(Pos), Angle(0.f),UnitVec(glm::vec3(0.f,0.f,1.f))
{
	this->Ex.x = DimXYZ / 2;
	this->Ex.y = DimXYZ / 2;
	this->Ex.z = DimXYZ / 2;
}

OBB::OBB(glm::vec3 Pos, float DimX, float DimXZ)
	:ColShapes(Pos), Angle(0.f), UnitVec(glm::vec3(0.f, 0.f, 1.f))
{
	this->Ex.x = DimX/2;
	this->Ex.y = DimXZ/2;
	this->Ex.z = DimXZ/2;
}

OBB::OBB(glm::vec3 Pos, float DimX, float DimY, float DimZ)
	:ColShapes(Pos), Angle(0.f), UnitVec(glm::vec3(0.f, 0.f, 1.f))
{
	this->Ex.x = DimX/2;
	this->Ex.y = DimY/2;
	this->Ex.z = DimZ/2;
}

OBB::~OBB()
{
}

glm::vec3 OBB::GetUnitVec()
{
	return this->UnitVec;
}

float OBB::GetAngle()
{
	return this->Angle;
}

void OBB::SetAngle(float SetAngle)
{
	this->Angle = SetAngle;
}

void OBB::SetUnitVec(glm::vec3 SetUnitVec)
{
	this->UnitVec = SetUnitVec;
}

std::vector<glm::vec3> OBB::GetSegments()
{
	glm::quat TempQuat = glm::angleAxis(glm::radians(this->Angle), this->UnitVec);
	glm::mat4 RotMat = glm::mat4_cast(TempQuat);

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
