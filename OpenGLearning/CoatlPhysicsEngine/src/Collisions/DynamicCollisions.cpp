#include "DynamicCollisions.h"
using namespace CoatlPhysicsEngine;
DynamicCollisions::DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
	:BaseCols(Name,InitCols)
{

}

DynamicCollisions::~DynamicCollisions()
{

}
