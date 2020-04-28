#include "Bodies.h"
using namespace CoatlPhysicsEngine;


Bodies::Bodies(int InitID)
{
	this->ID = InitID;
}

Bodies::Bodies( std::shared_ptr<ColShapes> InitShapes, int InitID)
{
	this->ID = InitID;
	this->BodyInf.push_back(std::make_shared<BodyParts>());
	this->BodyInf[0]->BodPart = InitShapes;
}

Bodies::Bodies( std::vector< std::shared_ptr<ColShapes>> InitShapes, int InitID)
{
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

void Bodies::AddShapes(std::shared_ptr<ColShapes> NewShape)
{

}

void Bodies::UpDateBodiesInf(bool ColUpdate, std::shared_ptr<Bodies> Body)
{
	int BodyId = Body->GetID();	
	std::vector<int>::iterator ii = std::find(this->OtherBods.begin(), this->OtherBods.end(), BodyId);
	if (ii != OtherBods.end())
	{
		if (!ColUpdate)
			this->OtherBods.erase(ii);		
	}
	else
	{
		if (ColUpdate)
			this->OtherBods.push_back(BodyId);
	}

}

int Bodies::GetID()
{
	return this->ID;
}
void Bodies::SetPosition(glm::vec3 NewPos)
{
	this->BodyInf[0]->BodPart->SetPos(NewPos);
}

glm::vec3 Bodies::GetPos()
{
	return this->BodyInf[0]->BodPart->GetPos();
}

std::shared_ptr<ColShapes> CoatlPhysicsEngine::Bodies::GetShapes()
{
	return this->BodyInf[0]->BodPart;
}

std::vector<int> CoatlPhysicsEngine::Bodies::GetAllCol()
{
	return this->OtherBods;
}
