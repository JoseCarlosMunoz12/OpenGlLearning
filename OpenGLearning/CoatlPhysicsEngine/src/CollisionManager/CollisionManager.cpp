#include "CollisionManager.h"
using namespace CoatlPhysicsEngine;

CollisionManager::CollisionManager()
	:S_Col(std::make_unique<SphereColSphere>())
{

}

CollisionManager::~CollisionManager()
{

}
