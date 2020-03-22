#include "StaticCollisions.h"
using namespace CoatlPhysicsEngine;

StaticCollisions::StaticCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
{
	this->ColMan = InitCols;
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
			std::shared_ptr<ColShapes> Val = AllStatics[ii]->GetShapes();
			std::shared_ptr<ColShapes> Val0 = AllStatics[jj]->GetShapes();
			bool Collied = false;
			
		}
	}
	
}

void StaticCollisions::AddNewBody(glm::vec3 InitPos, std::shared_ptr<ColShapes> NewShape)
{
	this->AllStatics.push_back(std::make_shared<Bodies>(InitPos,NewShape,this->NewCurId));
	this->NewCurId++;
}

void StaticCollisions::AddNewBody(glm::vec3 InitPos, std::vector<std::shared_ptr<ColShapes>> NewShapes)
{
	for (auto& ii : NewShapes)
	{
		this->AllStatics.push_back(std::make_shared<Bodies>(InitPos,ii, this->NewCurId));
	}
	this->NewCurId++;
}

std::string CoatlPhysicsEngine::StaticCollisions::GetName()
{
	return this->Name;
}

std::vector<std::shared_ptr<Bodies>> CoatlPhysicsEngine::StaticCollisions::GetAllBodies()
{
	return this->AllStatics;
}

std::shared_ptr<Bodies> CoatlPhysicsEngine::StaticCollisions::GetABody(int ID)
{
	std::shared_ptr<Bodies> Temp;
	for (auto& ii : this->AllStatics)
	{
		if (ii->GetID() == ID)
		{
			Temp = ii;
			break;
		}
	}
	return Temp;
}
