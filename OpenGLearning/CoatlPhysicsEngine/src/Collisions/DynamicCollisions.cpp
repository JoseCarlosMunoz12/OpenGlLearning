#include "DynamicCollisions.h"
using namespace CoatlPhysicsEngine;
void DynamicCollisions::CalcPhysics(std::weak_ptr<Bodies> Bod0, std::weak_ptr<Bodies> Bod1)
{

}

DynamicCollisions::DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
	:BaseCols(Name,InitCols),Phy_Col(),Phy_Motion(),
	Ext(100.f), AlgoType(Alg_Type::B_F), B_Ex(4.f)
{

}

DynamicCollisions::~DynamicCollisions()
{

}

void DynamicCollisions::CheckCollision(std::shared_ptr<StaticCollisions> Statics,glm::vec3 Grav,float dt)
{
	//Check Collision with The Terrain
	if (!this->Ter.expired())
	{
		for (auto& jj : AllBods)
		{
			std::vector<std::shared_ptr<Bodies>> Quer = Ter.lock()->GetTerrs(jj->GetPos(), 1);
			std::shared_ptr<Particle> Temp = jj->GetSpecificBodyPart(0)->BodParticle;
			if (Temp)
			{
				bool Collided = false;
				glm::vec3 PrevPos = jj->GetPos();
				glm::vec3 Bod_Vel= Temp->GetVel();
				float DivDt = dt / 50;
				float F_dt = dt;
				for (auto& ii : Quer)
				{
					for (int kk = 0; kk <= 50; kk++)
					{
						F_dt = DivDt * (float)kk;
						jj->SetPosition(PrevPos + Bod_Vel  * F_dt);
						Collided = this->ColBods(jj, ii);
						if (Collided)
						{
							if (glm::abs(Bod_Vel.z) > 0.2f)
							{
								jj->SetPosition(PrevPos + Bod_Vel * DivDt * (float)(kk - 1));
								Temp->SetVel(glm::vec3(0.f, 0.f, -Bod_Vel.z / 2));
							}
							else
							{
								Temp->SetVel(glm::vec3(0.f));
							}
							break;
						}
					}
					if (Collided)
						break;
				}
				if (!Collided)
					jj->SetPosition(Temp->UpdatePos(PrevPos, Grav,F_dt*.8));
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
	if (AlgoCheck)
		AlgoCheck->Erase();
	//make approriate Algorithm
	switch (AlgoType)
	{
	case Alg_Type::B_F:
		this->AlgoCheck = std::make_unique<B_Force>();
		break;
	case Alg_Type::Q_T:
		this->AlgoCheck = std::make_unique<QuadTree>(glm::vec3(0.f), Ext);
	case Alg_Type::O_T:
		this->AlgoCheck = std::make_unique<OctoTree>(glm::vec3(0.f), Ext);
	default:
		break;
	}
	////Add bodies into Algorithm
	for (auto& jj : AllBods)
	{
		this->AlgoCheck->Insert(jj);
	}
	////Check Self Collision
	//for (auto& jj : AllBods)
	//{
	//	std::vector<std::shared_ptr<Bodies>> Quer = this->AlgoCheck->GetQueries(jj, B_Ex);
	//	for (auto& ii : Quer)
	//	{
	//		if (jj->GetID() != ii->GetID())
	//		{
	//			this->ColBods(jj, ii);
	//		}
	//	}
	//}
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
