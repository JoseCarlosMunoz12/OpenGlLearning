#include "Bodies.h"
using namespace CoatlPhysicsEngine;


Bodies::Bodies(int InitID)
{
	this->ID = InitID;
}

Bodies::Bodies( std::shared_ptr<ColShapes> InitShapes, int InitID)
{
	this->ID = InitID;
	int Count = this->BodyInf.size();
	this->BodyInf.push_back(std::make_shared<BodyParts>()); 
	this->BodyInf[Count]->BodPart = InitShapes;

}

Bodies::Bodies( std::vector< std::shared_ptr<ColShapes>> InitShapes, int InitID)
{
	this->ID = InitID;
	int Count = this->BodyInf.size();
	for (auto& ii : InitShapes)
	{
		this->BodyInf.push_back(std::make_shared<BodyParts>());
		this->BodyInf[Count]->BodPart = ii;
		Count++;
	}
}

Bodies::~Bodies()
{
}

void Bodies::AddShapes(std::shared_ptr<ColShapes> NewShape)
{
	this->BodyInf.push_back(std::make_shared<BodyParts>());
	int Size = this->BodyInf.size() - 1;
	this->BodyInf[Size]->BodPart = NewShape;
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

void Bodies::ClearColsInf()
{
	this->OtherBods.clear();
}

int Bodies::GetID()
{
	return this->ID;
}

void Bodies::CalcPhysics(float dt)
{
	if (this->BodyInf[0]->BodParticle)
	{	glm::vec3 NewPos = this->BodyInf[0]->BodParticle->UpdatePos(this->GetPos(), glm::vec3(0.f), dt);
		this->SetPosition(NewPos);
	}
}

void Bodies::SetPosition(glm::vec3 NewPos)
{
	this->BodyInf[0]->BodPart->SetPos(NewPos);
}

void Bodies::SetParticle(int ShapeID)
{
	this->BodyInf[ShapeID]->BodParticle = std::make_shared<Particle>();
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

std::vector<std::shared_ptr<BodyParts>> CoatlPhysicsEngine::Bodies::GetBodyParts()
{
	return this->BodyInf;
}

std::shared_ptr<BodyParts> CoatlPhysicsEngine::Bodies::GetSpecificBodyPart(int ID)
{
	return this->BodyInf[ID];
}
