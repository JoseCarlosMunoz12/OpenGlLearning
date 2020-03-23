#include "ColShapes.h"
using namespace CoatlPhysicsEngine;

SHAPE_TYPE CoatlPhysicsEngine::ColShapes::GetType()
{
	return SHAPE_TYPE::S_BASE;
}

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
