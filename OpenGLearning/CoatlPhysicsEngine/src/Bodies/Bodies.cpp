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

bool Bodies::DetectCollision(Bodies* OtherBody)
{
	ColShapes* MyShape = this->BodyInf[0]->BodPart;
	ColShapes* OtherShape = OtherBody->BodyInf[0]->BodPart;

	return false;

}

void CoatlPhysicsEngine::Bodies::UpDateBodiesInf(Bodies* Body)
{
}
