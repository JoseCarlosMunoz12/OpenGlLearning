#include "OBB.h"
using namespace CoatlPhysicsEngine;
OBB::OBB(glm::vec3 Pos, float DimXYZ)
	:ColShapes(Pos), Angle(0.f),UnitVec(glm::vec3(0.f,0.f,1.f))
{
	this->Ex.x = DimXYZ/2;
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
	return std::vector<glm::vec3>();
}

glm::vec3 OBB::GetLenghts()
{
	return this->Ex;
}
