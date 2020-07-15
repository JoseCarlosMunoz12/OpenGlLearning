#include "ColShapes.h"
using namespace CoatlPhysicsEngine;

ColShapes::ColShapes(glm::vec3 SetPos)
{
	this->Pos = SetPos;
	this->QuatAngle = glm::angleAxis(0.f, glm::vec3(0.f, 0.f, 1.f));
	this->ParMat = glm::mat4(1.f);
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

void CoatlPhysicsEngine::ColShapes::SetPos(glm::vec3 NewPos)
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

glm::mat4 ColShapes::ShapeMatrix()
{
	glm::mat4 R = glm::mat4(1.f);
	glm::mat4 Rot = glm::mat4_cast(this->QuatAngle);
	R *= Rot;
	return glm::translate(R,this->Pos);
}

glm::mat4 CoatlPhysicsEngine::ColShapes::GetParMatrix()
{
	return this->ParMat;
}

void ColShapes::SetMatrix(glm::mat4 P)
{
	this->ParMat = P;
}
