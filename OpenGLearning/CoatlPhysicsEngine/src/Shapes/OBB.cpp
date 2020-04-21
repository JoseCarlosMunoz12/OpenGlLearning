#include "OBB.h"
using namespace CoatlPhysicsEngine;
OBB::OBB(glm::vec3 Pos, float DimXYZ)
	:ColShapes(Pos), Angle(0.f),UnitVec(glm::vec3(0.f,0.f,1.f))
{
	this->Ex.x = DimXYZ/2;
	this->Ex.y = DimXYZ / 2;
	this->Ex.z = DimXYZ / 2;
}

OBB::OBB(glm::vec3 Pos, float DimX, float DimXY)
	:ColShapes(Pos), Angle(0.f), UnitVec(glm::vec3(0.f, 0.f, 1.f))
{

}

OBB::OBB(glm::vec3 Pos, float DimX, float DimY, float DimZ)
	:ColShapes(Pos), Angle(0.f), UnitVec(glm::vec3(0.f, 0.f, 1.f))
{

}

OBB::~OBB()
{
}

glm::vec3 OBB::GetUnitVec()
{
	return glm::vec3();
}

float OBB::GetAngle()
{
	return 0.0f;
}

void OBB::SetAngle(float SetAngle)
{
}

void OBB::SetUnitVec(glm::vec3 SetUnitVec)
{
}

std::vector<glm::vec3> OBB::GetSegments()
{
	return std::vector<glm::vec3>();
}

glm::vec3 OBB::GetLenghts()
{
	return glm::vec3();
}
