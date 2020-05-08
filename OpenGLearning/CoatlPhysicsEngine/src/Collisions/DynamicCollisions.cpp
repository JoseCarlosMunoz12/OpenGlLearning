#include "DynamicCollisions.h"
using namespace CoatlPhysicsEngine;
DynamicCollisions::DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
	:BaseCols(Name,InitCols),AlgoType(Alg_Type::B_F)
{

}

DynamicCollisions::~DynamicCollisions()
{

}

void DynamicCollisions::CheckCollision(std::shared_ptr<StaticCollisions> Statics)
{
	for (auto& jj : AllBods)
	{
		Statics->CheckCol(jj);
	}
}

std::vector<std::shared_ptr<Bodies>> DynamicCollisions::GetBodies()
{
	return  this->AllBods;
}
