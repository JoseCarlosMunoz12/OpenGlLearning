#include "ColShapes.h"
using namespace CoatlPhysicsEngine;

ColShapes::ColShapes(glm::vec3 SetPos)
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

void CoatlPhysicsEngine::ColShapes::SetPos(glm::vec3 NewPos)
{
	this->Pos = NewPos;
}
