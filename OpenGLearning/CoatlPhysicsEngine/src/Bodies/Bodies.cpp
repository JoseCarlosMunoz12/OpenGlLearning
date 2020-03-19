#include "Bodies.h"
using namespace CoatlPhysicsEngine;


Bodies::Bodies(int InitID)
{
	this->ID = InitID;
}

Bodies::Bodies(glm::vec3 NewPos, std::shared_ptr<ColShapes> InitShapes, int InitID)
{
	this->Pos = NewPos;
	this->ID = InitID;
	this->BodyInf.push_back(std::make_shared<BodyParts>());
	this->BodyInf[0]->BodPart = InitShapes;
}

Bodies::Bodies(glm::vec3 NewPos, std::vector<std::shared_ptr<ColShapes>> InitShapes, int InitID)
{
	this->Pos = NewPos;
	this->ID = InitID;
	int Count = 0;
	for (auto& ii : InitShapes)
	{
		this->BodyInf.push_back(std::make_shared<BodyParts>());
		this->BodyInf[Count]->BodPart = ii;
	}
}

Bodies::~Bodies()
{
}

void Bodies::AddShapes(ColShapes NewShape)
{

}

bool Bodies::DetectCollision(std::shared_ptr<Bodies>OtherBody)
{
	return false;

}

void Bodies::UpDateBodiesInf(std::shared_ptr<Bodies> Body)
{
	Body->GetID();
}

int Bodies::GetID()
{
	return this->ID;
}
void Bodies::SetPosition(glm::vec3 NewPos)
{
	this->Pos = NewPos;
}

glm::vec3 CoatlPhysicsEngine::Bodies::GetPos()
{
	return this->Pos;
}
