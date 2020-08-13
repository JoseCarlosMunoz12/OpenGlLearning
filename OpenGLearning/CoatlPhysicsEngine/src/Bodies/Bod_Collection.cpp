#include "Bod_Collection.h"
using namespace CoatlPhysicsEngine;

Collection::Collection()
{
}

Collection::~Collection()
{
}

void Collection::MakeJoint(std::shared_ptr<ColJoints> NewCol)
{
	this->Jnts.push_back(NewCol);
}

void Collection::DeleteJoint(int ID)
{
	this->Jnts.erase(Jnts.begin() + ID);
}

std::vector<std::shared_ptr<ColJoints>> CoatlPhysicsEngine::Collection::GetJoints()
{
	return this->Jnts;
}
