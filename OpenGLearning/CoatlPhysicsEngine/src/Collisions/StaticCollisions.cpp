#include "StaticCollisions.h"
using namespace CoatlPhysicsEngine;

StaticCollisions::StaticCollisions(std::string Name, glm::vec3 InitPos)
	:Pos(InitPos)
{
	this->Name = Name;
}

StaticCollisions::~StaticCollisions()
{
	std::cout << "Good bye from " + Name + "\n";
}

void StaticCollisions::ShowName()
{
	std::cout << "The Name is "+ Name + "\n";
}
glm::vec3 StaticCollisions::GetPos()
{
	return this->Pos;
}