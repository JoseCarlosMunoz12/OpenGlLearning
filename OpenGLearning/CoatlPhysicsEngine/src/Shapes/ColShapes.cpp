#include "ColShapes.h"
using namespace CoatlPhysicsEngine;

ColShapes::ColShapes(glm::vec3 SetPos)
	:Pivot(glm::vec3(0.f))
{
	this->Pos = SetPos;
	this->QuatAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));

}

ColShapes::~ColShapes()
{
}

glm::vec3 ColShapes::GetPos()
{
	return this->Pos;
}

glm::quat ColShapes::GetQuatAngle()
{
	return this->QuatAngle;
}

void ColShapes::SetPos(glm::vec3 NewPos)
{
	this->Pos = NewPos;
}

glm::vec3 ColShapes::Support(glm::vec3 Dir)
{
	return glm::vec3(0.f);
}

glm::vec3 ColShapes::EPA_Support(glm::vec3 Dir)
{
	return glm::vec3();
}

std::vector<glm::vec3> ColShapes::GetVertices()
{
	return std::vector<glm::vec3>();
}

std::vector<glm::vec3> ColShapes::GetNormals()
{
	return std::vector<glm::vec3>();
}

glm::mat3 ColShapes::GetInertia(float Mass)
{
	return glm::mat3(0.f);
}

