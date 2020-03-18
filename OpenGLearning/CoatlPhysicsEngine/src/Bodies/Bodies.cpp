#include "Bodies.h"
using namespace CoatlPhysicsEngine;


Bodies::Bodies(int InitID)
{
	this->ID = InitID;
}

Bodies::Bodies(std::shared_ptr<ColShapes> InitShapes, int InitID)
{
	this->ID = InitID;
	std::shared_ptr<BodyParts> Temp;
	Temp->BodPart = InitShapes;
	this->BodyInf.push_back(Temp);
}

Bodies::~Bodies()
{
}

bool Bodies::DetectCollision(std::shared_ptr<Bodies>OtherBody)
{
	return false;

}

void CoatlPhysicsEngine::Bodies::UpDateBodiesInf(std::shared_ptr<Bodies> Body)
{

}
