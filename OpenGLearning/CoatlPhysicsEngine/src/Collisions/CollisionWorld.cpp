#include "CollisionWorld.h"

using namespace CoatlPhysicsEngine;

CollisionWorld::CollisionWorld(std::string SetWName)
	:WorldName(SetWName),Gravity(glm::vec3(0.f,0.f,-9.81f))
{
}

CollisionWorld::CollisionWorld(CollisionWorldSetup SetUp)
	:WorldName(SetUp.Name),Gravity(SetUp.Gravity)
{
}

CollisionWorld::~CollisionWorld()
{	
	delete Statics;
	Statics = nullptr;
	delete Dynamics;
	Dynamics = nullptr;
}

std::string CollisionWorld::ShowName()
{
	return this->WorldName;
}

glm::vec3 CollisionWorld::GetGravity()
{
	return this->Gravity;
}

void CollisionWorld::ChangeName(std::string NewName)
{
	this->WorldName = NewName;
}

StaticCollisions* CoatlPhysicsEngine::CollisionWorld::CreateStaticCol(std::string Name)
{
	this->Statics = new StaticCollisions(Name);
	return this->Statics;
}

