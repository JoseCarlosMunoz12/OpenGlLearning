#include "ColShapes.h"
using namespace CoatlPhysicsEngine;

ColShapes::ColShapes(glm::vec3 SetPos)
{
	this->Pos = SetPos;
}

ColShapes::~ColShapes()
{

}

std::string ColShapes::GetShapeType()
{
	return "Base";
}

glm::vec3 CoatlPhysicsEngine::ColShapes::GetPos()
{
	return this->Pos;
}
