#include "StaticCollisions.h"
using namespace CoatlPhysicsEngine;

StaticCollisions::StaticCollisions(std::string Name)
{
	this->Name = Name;
}

StaticCollisions::~StaticCollisions()
{	
}

void StaticCollisions::UpdateCollisionCheck()
{
	int Size = AllStatics.size();
	for (int ii = 0; ii < Size; ii++)
	{
		for (int jj = ii + 1; jj < Size; jj++)
		{
			AllStatics[ii]->DetectCollision(AllStatics[jj]);
		}

	}
	
}

std::string CoatlPhysicsEngine::StaticCollisions::GetName()
{
	return this->Name;
}
