#include "CollisionWorld.h"

using namespace CoatlPhysicsEngine;

CollisionWorld::CollisionWorld(std::string SetWName)
	:WorldName(SetWName)
{
}

CoatlPhysicsEngine::CollisionWorld::CollisionWorld(CollisionWorldSetup SetUp)
	:WorldName(SetUp.Name)
{
	this->Gravity = SetUp.Gravity;
}

CollisionWorld::~CollisionWorld()
{

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
