#include "StaticCollisions.h"
using namespace CoatlPhysicsEngine;

StaticCollisions::StaticCollisions(std::string Name)
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