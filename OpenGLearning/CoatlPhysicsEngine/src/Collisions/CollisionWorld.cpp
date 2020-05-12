#include "CollisionWorld.h"

using namespace CoatlPhysicsEngine;

CollisionWorld::CollisionWorld(std::string SetWName)
	:WorldName(SetWName),Gravity(glm::vec3(0.f,0.f,-9.81f))
{
	this->ColMan = std::make_shared<CollisionManager>();
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

void CollisionWorld::CreateStaticCol(std::string Name)
{
	if (!Statics)
	{
		this->Statics = std::make_shared<StaticCollisions>(Name, this->ColMan);
		this->Statics->SetTerrain(this->Ter);
	}
}

void CollisionWorld::CreateDynamicCol(std::string Name)
{
	if (!Dynamics)
	{
		this->Dynamics = std::make_shared<DynamicCollisions>(Name, this->ColMan);
		this->Dynamics->SetTerrain(this->Ter);
	}
}

std::shared_ptr<StaticCollisions> CollisionWorld::GetCollision()
{
	if (this->Statics)
	{
		return this->Statics;
	}
	return NULL;
}

std::shared_ptr<DynamicCollisions> CollisionWorld::GetDynCol()
{
	if (this->Dynamics)
		return this->Dynamics;
	return NULL;
}

void CoatlPhysicsEngine::CollisionWorld::DeleteStatics()
{
	this->Statics.reset();
}

void CoatlPhysicsEngine::CollisionWorld::DeleteDynamics()
{
	this->Dynamics.reset();
}

void CoatlPhysicsEngine::CollisionWorld::UpdateWorld(float dt)
{
	//check where the Statics are if touching the Terrain
	if (this->Statics && CheckStatics)
	{
		this->Statics->UpdateCollisionCheck();
	}
	//Does Physics Maths and Dynamic Collisions
	if (this->Dynamics)
	{
		this->Dynamics->CheckCollision(this->Statics,this->Gravity,dt);
	}
}

void CollisionWorld::SetTerrain(std::vector<glm::vec3> Ver, std::vector<int> Ind, float Dim)
{
	this->Ter = std::make_shared<Terrain>(Ver, Ind, Dim);
	if (this->Statics)
		this->Statics->SetTerrain(Ter);
	if (this->Dynamics)
		this->Dynamics->SetTerrain(Ter);
}

void CollisionWorld::ToggleStaticCheck()
{
	this->CheckStatics = !this->CheckStatics;
}

bool CollisionWorld::StaticCheckStatus()
{
	return this->CheckStatics;
}
