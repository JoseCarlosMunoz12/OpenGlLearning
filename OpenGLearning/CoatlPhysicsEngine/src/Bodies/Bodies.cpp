#include "Bodies.h"
using namespace CoatlPhysicsEngine;

Bodies::Bodies()
{
}

Bodies::~Bodies()
{
	for (auto& ii : this->BodyInf)
	{
		delete ii;
	}
}

bool Bodies::DetecCollision(Bodies* OtherBody)
{
	ColShapes* MyShape = this->BodyInf[0]->BodPart;
	ColShapes* OtherShape = OtherBody->BodyInf[0]->BodPart;

}
