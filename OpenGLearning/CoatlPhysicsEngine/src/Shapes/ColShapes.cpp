#include "ColShapes.h"
using namespace CoatlPhysicsEngine;

ColShapes::ColShapes(glm::vec3 SetPos)
{
	this->Pos = SetPos;
}

ColShapes::~ColShapes()
{
}

glm::vec3 CoatlPhysicsEngine::ColShapes::GetPos()
{
	return this->Pos;
}

void CoatlPhysicsEngine::ColShapes::SetPos(glm::vec3 NewPos)
{
	this->Pos = NewPos;
}
