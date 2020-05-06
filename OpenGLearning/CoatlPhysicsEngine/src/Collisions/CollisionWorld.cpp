#include "CollisionWorld.h"

using namespace CoatlPhysicsEngine;

CollisionWorld::CollisionWorld(std::string SetWName)
	:WorldName(SetWName),Gravity(glm::vec3(0.f,0.f,-9.81f))
{
}

CollisionWorld::CollisionWorld(CollisionWorldSetup SetUp)
	:WorldName(SetUp.Name),Gravity(SetUp.Gravity)
{
	this->ColMan = std::make_shared<CollisionManager>();
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

void CoatlPhysicsEngine::CollisionWorld::CreateStaticCol(std::string Name)
{
	if (!Statics)
	{
		this->Statics = std::make_shared<StaticCollisions>(Name, this->ColMan);
		this->Statics->SetTerrain(this->Ter);
	}
}

std::shared_ptr<StaticCollisions> CoatlPhysicsEngine::CollisionWorld::GetCollision()
{
	if (this->Statics)
	{
		return this->Statics;
	}
	return NULL;
}

void CoatlPhysicsEngine::CollisionWorld::DeleteStatics()
{
	this->Statics.reset();
}

void CoatlPhysicsEngine::CollisionWorld::UpdateWorld()
{
	if (this->Statics)
	{
		this->Statics->UpdateCollisionCheck();
	}
}

void CollisionWorld::SetTerrain(std::vector<glm::vec3> Ver, std::vector<int> Ind, float Dim)
{
	this->Ter = std::make_shared<Terrain>(Ver, Ind, Dim);
	if (this->Statics)
		this->Statics->SetTerrain(Ter);
}
