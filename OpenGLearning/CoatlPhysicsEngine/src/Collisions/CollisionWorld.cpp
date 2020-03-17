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
	if (Statics)
		delete Statics;
	Statics = NULL;
	if (Dynamics)
		delete Dynamics;
	Dynamics = NULL;
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

void CoatlPhysicsEngine::CollisionWorld::CreateStaticCol(std::string Name)
{
	if(!Statics)
		this->Statics = new StaticCollisions(Name);
}

StaticCollisions* CoatlPhysicsEngine::CollisionWorld::GetCollision()
{
	if (this->Statics)
	{
		return this->Statics;
	}
	return NULL;
}

void CoatlPhysicsEngine::CollisionWorld::DeleteStatics()
{
	delete this->Statics;
	this->Statics = NULL;
}
