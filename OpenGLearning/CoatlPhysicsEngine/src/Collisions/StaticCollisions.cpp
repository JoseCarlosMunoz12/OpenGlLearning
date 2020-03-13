#include "StaticCollisions.h"
using namespace CoatlPhysicsEngine;

StaticCollisions::StaticCollisions(std::string Name, glm::vec3 InitPos)
	:Pos(InitPos)
{

}

StaticCollisions::~StaticCollisions()
{

}
glm::vec3 StaticCollisions::GetPos()
{
	return this->Pos;
}

void StaticCollisions::SetPos(glm::vec3 NewPos)
{
	this->Pos = NewPos;
}
