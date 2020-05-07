#include "StaticCollisions.h"
using namespace CoatlPhysicsEngine;

StaticCollisions::StaticCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
	:BaseCols(Name, InitCols), Ext(100.f), AlgoType(Alg_Type::B_F), B_Ex(4.f)
{
}

StaticCollisions::~StaticCollisions()
{	
}

void StaticCollisions::UpdateCollisionCheck()
{
	//Collision with World
	//------------------------
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
	//make approriate Algorithm
	switch (AlgoType)
	{
	case Alg_Type::B_F:
		this->AlgoCheck = std::make_unique<B_Force>();
		break;
	case Alg_Type::Q_T:
		this->AlgoCheck = std::make_unique<QuadTree>(glm::vec3(0.f),Ext);
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
	//get queries and test them
	int Size = AllBods.size();
	//O(nlog) for Quadtree and OctoTree
	//O(n^2) for B_Force
	for (int ii = 0; ii < Size; ii++)
	{
		std::vector<std::shared_ptr<Bodies>> Quer = this->AlgoCheck->GetQueries(AllBods[ii], Ext);
		for (auto& kk : Quer)
		{
			this->ColBods(AllBods[ii], kk);
		}
	}
}

void StaticCollisions::AddNewBody( std::shared_ptr<ColShapes> NewShape)
{
	this->AllBods.push_back(std::make_shared<Bodies>(NewShape,this->NewCurId));
	this->NewCurId++;
}

void StaticCollisions::AddNewBody(std::vector<std::shared_ptr<ColShapes>> NewShapes)
{
	for (auto& ii : NewShapes)
	{
		this->AllBods.push_back(std::make_shared<Bodies>(ii, this->NewCurId));
	}
	this->NewCurId++;
}

std::string StaticCollisions::GetName()
{
	return this->Name;
}

std::vector<std::weak_ptr<Bodies>> StaticCollisions::GetAllBodies()
{
	std::vector<std::weak_ptr<Bodies>> Temp;
	for (auto& jj : this->AllBods)
	{
		Temp.push_back(jj);
	}
	return Temp;
}

std::shared_ptr<Bodies> StaticCollisions::GetABody(int ID)
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

Alg_Type StaticCollisions::GetType()
{
	return this->AlgoType;
}

void StaticCollisions::SetNewType(Alg_Type NewType)
{
	this->AlgoType = NewType;
}

void StaticCollisions::SetTerrain(std::shared_ptr<Terrain> NewTer)
{
	this->Ter = NewTer;
}
