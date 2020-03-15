#include "StaticCollisions.h"
using namespace CoatlPhysicsEngine;

StaticCollisions::StaticCollisions(std::string Name, glm::vec3 InitPos)
{

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
			AllStatics[ii]->DetecCollision(AllStatics[jj]);
		}

	}
	
}
