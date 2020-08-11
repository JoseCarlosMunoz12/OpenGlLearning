#include "KinematicsCollisions.h"
using namespace CoatlPhysicsEngine;

KinematicsCollisions::KinematicsCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
	:BaseCols(Name,InitCols), Ext(100.f), AlgoType(Alg_Type::O_T), B_Ex(4.f)
{

}

KinematicsCollisions::~KinematicsCollisions()
{
}

std::vector<std::shared_ptr<Bodies>> KinematicsCollisions::GetBods(std::shared_ptr<Bodies> Bod)
{
	//make approriate Algorithm
	switch (AlgoType)
	{
	case Alg_Type::B_F:
		this->AlgoCheck = std::make_unique<B_Force_Self>();
		break;
	case Alg_Type::Q_T:
		this->AlgoCheck = std::make_unique<QuadTree>(glm::vec3(0.f), Ext);
	case Alg_Type::O_T:
		this->AlgoCheck = std::make_unique<OctoTree>(glm::vec3(0.f), Ext);
	default:
		break;
	}
	//Add bodies into Algorithm
	for (auto& jj : AllBods)
	{
		this->AlgoCheck->Insert(jj);
	}
	//get queries and test them
	return this->AlgoCheck->GetQueries(Bod, Ext);
}

void KinematicsCollisions::UpdateBodies(float dt)
{

}
