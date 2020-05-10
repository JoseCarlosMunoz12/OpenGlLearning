#include "DynamicCollisions.h"
using namespace CoatlPhysicsEngine;
DynamicCollisions::DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
	:BaseCols(Name,InitCols), Ext(100.f), AlgoType(Alg_Type::B_F), B_Ex(4.f)
{

}

DynamicCollisions::~DynamicCollisions()
{

}

void DynamicCollisions::CheckCollision(std::shared_ptr<StaticCollisions> Statics)
{
	//Check Collision with The Terrain
	if (!this->Ter.expired())
	{
		for (auto& jj : AllBods)
		{
			jj->ClearColsInf();
			std::vector<std::shared_ptr<Bodies>> Quer = Ter.lock()->GetTerrs(jj->GetPos(), 1);
			for (auto& ii : Quer)
			{
				this->ColBods(jj, ii);
			}
		}
	}
	//Check Collision with Static Bodies
	if (Statics)
	{
		for (auto& jj : AllBods)
		{
			Statics->CheckCol(jj);
		}

	}
	//Check Collisions with each other
	//make approriate Algorithm
	switch (AlgoType)
	{
	case Alg_Type::B_F:
		this->AlgoCheck = std::make_unique<B_Force_Self>();
		break;
	case Alg_Type::Q_T:
		this->AlgoCheck = std::make_unique<QuadTree>(glm::vec3(0.f), Ext);
	case Alg_Type::O_T:
		this->AlgoCheck = std::make_unique<OctoTree>(glm::vec3(0.f), Ext);
	default:
		break;
	}
	//Add bodies into Algorithm
	for (auto& jj : AllBods)
	{
		this->AlgoCheck->Insert(jj);
	}
	//Calculate All Physics Interaction
	for (auto& jj : AllBods)
	{
		std::vector<std::shared_ptr<Bodies>> Quer = this->AlgoCheck->GetQueries(jj, B_Ex);
		for (auto& ii : Quer)
		{
			this->ColBods(jj, ii);
		}
	}
}

void DynamicCollisions::UpdatePhysics(glm::vec3 Acc,float dt)
{
	//update Physics
	for (auto& jj : AllBods)
		jj->CalcPhysics(Acc,dt);
}

void DynamicCollisions::AddNewBody(std::shared_ptr<ColShapes> NewShape)
{
	this->AllBods.push_back(std::make_shared<Bodies>(NewShape, this->NewCurId));
	this->NewCurId++;
}

void DynamicCollisions::AddNewBody(std::vector<std::shared_ptr<ColShapes>> NewShapes)
{
	for (auto& jj : NewShapes)
		AddNewBody(jj);
}

std::string DynamicCollisions::GetName()
{
	return this->Name;
}

std::vector<std::weak_ptr<Bodies>> DynamicCollisions::GetAllBodies()
{
	std::vector<std::weak_ptr<Bodies>> Temp;
	for (auto& jj : this->AllBods)
	{
		Temp.push_back(jj);
	}
	return Temp;
}

std::shared_ptr<Bodies> DynamicCollisions::GetABody(int ID)
{
	std::shared_ptr<Bodies> Temp;
	for (auto& ii : this->AllBods)
	{
		if (ii->GetID() == ID)
		{
			Temp = ii;
			break;
		}
	}
	return Temp;
}

Alg_Type DynamicCollisions::GetType()
{
	return this->AlgoType;
}

void DynamicCollisions::SetNewType(Alg_Type NewType)
{
	this->AlgoType = NewType;
}

void DynamicCollisions::SetTerrain(std::shared_ptr<Terrain> NewTer)
{
	this->Ter = NewTer;
}
