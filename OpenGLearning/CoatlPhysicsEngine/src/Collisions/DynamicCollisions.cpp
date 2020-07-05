#include "DynamicCollisions.h"
using namespace CoatlPhysicsEngine;

bool DynamicCollisions::BinColDetection(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1,
	glm::vec3 Vel, glm::vec3 Pos,
	float t0, float t1, float& NewDt)
{
	if ((t1 - t0 ) < EPSILON)
	{
		NewDt = t0;
		return true;
	}
	float Mid = t0 + (t1 - t0) / 2.f;
	glm::vec3 TempPos0 = MATH::ClosestPoint_Seg({Pos + Vel * t0,Pos + Vel * t1}, Bod1->GetPos());
	Bod0->SetPosition(TempPos0);
	if (!this->ColBods(Bod0, Bod1))
	{
		return false;
	}
	if (BinColDetection(Bod0, Bod1, Vel,Pos, t0, Mid, NewDt))
	{
		return true;
	}
	return BinColDetection(Bod0, Bod1, Vel,Pos, Mid, t1, NewDt);
}

void DynamicCollisions::CalcPhysics(std::weak_ptr<Bodies> Bod0, std::weak_ptr<Bodies> Bod1)
{

}

bool DynamicCollisions::ContainsManifold(std::vector<std::shared_ptr<Manifold>> ColRel, std::shared_ptr<Manifold> NewMan)
{
	for (auto& jj : ColRel)
	{
		bool F_Same = false;
		bool S_Same = false;
		if (jj->Bod0->GetID() == NewMan->Bod0->GetID() || jj->Bod1->GetID() == NewMan->Bod0->GetID())
			F_Same = true;
		if (jj->Bod0->GetID() == NewMan->Bod1->GetID() || jj->Bod1->GetID() == NewMan->Bod1->GetID())
			S_Same = true;
		if (S_Same && F_Same)
			return true;
	}
	return false;
}

DynamicCollisions::DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
	:BaseCols(Name,InitCols),
	Ext(100.f), AlgoType(Alg_Type::B_F), B_Ex(4.f)
{
	this->Col_Rel = std::make_unique<Col_Resolution>();
	this->Gravity = glm::vec3(0.f, 0.f, -9.81f);
	this->Grav_F_Manager = std::make_unique<Phy_Grav>(this->Gravity);
}

DynamicCollisions::~DynamicCollisions()
{

}

void DynamicCollisions::CheckCollision(std::shared_ptr<StaticCollisions> Statics,float dt)
{
	std::vector<std::shared_ptr<Manifold>> ColRel;
	//make approriate Algorithm
	if (AlgoCheck)
		AlgoCheck->Erase();
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
	//Add bodies into Algorithm
	for (auto& jj : AllBods)
	{
		this->AlgoCheck->Insert(jj);
	}
	//Find all Collisions and Calculate its Force Geneartors
	for (auto& jj : AllBods)
	{	
		std::shared_ptr<Particle> Temp = jj->GetSpecificBodyPart(0)->BodParticle;
		if (Temp)
		{
			//resetForces on the Object
			Temp->ResetForce();
			//Gravitaional Force
			Temp->AcumForce(this->Grav_F_Manager->GetForce(*Temp));
			// Drag Force
			/*this->F_Manager = std::make_unique<Phy_Bungee>(glm::vec3(0.f,0.f,10.f),1000,100,5);
			Temp->AcumForce(this->F_Manager->GetForce(*Temp));*/
			// Spring Force
			//this->F_Manager = std::make_unique<Phy_Drag>(1, 0);
			//Temp->AcumForce(this->F_Manager->GetForce(*Temp));
			glm::vec3 PrevPos = jj->GetPos();
			glm::vec3 Bod_Vel= Temp->GetVel();
			float F_dt = dt;
			//Check Collision with The Terrain
			if (!Ter.expired())
			{
				std::vector<std::shared_ptr<Bodies>> Quers = Ter.lock()->GetTerrs(jj->GetPos(), 1);
				if (!this->Ter.expired())
				{					
					for (auto& ii : Quers)
					{
						if (this->BinColDetection(jj, ii, Bod_Vel, PrevPos, 0, dt, F_dt))
						{
							std::shared_ptr<Manifold> T = this->Col_Rel->MakeManifold(jj, ii, 0);
							if (!this->ContainsManifold(ColRel, T))
								ColRel.push_back(T);
							Temp->AcumForce(-Gravity * Temp->GetMass());
							if (glm::abs(Bod_Vel.z) > 0.125f)
							{
								Temp->SetVel(glm::vec3(Bod_Vel.x, Bod_Vel.y,glm::abs(Bod_Vel.z	/2)));
							}
							else
							{
								Temp->SetVel(glm::vec3(Bod_Vel.x, Bod_Vel.y, 0.f));
							}
						}
					}
				}
			}
			//Check Collision with Static Bodies
			if (Statics)
			{
				std::vector<std::shared_ptr<Bodies>> Que = Statics->GetBods(jj);
				for (auto& ii : Que)
				{
					if (this->BinColDetection(jj, ii, Bod_Vel, PrevPos, 0, dt, F_dt))
					{
						std::shared_ptr<Manifold> T = this->Col_Rel->MakeManifold(jj, ii, 0);
						if (!this->ContainsManifold(ColRel, T))
							ColRel.push_back(T);
					}
					else
					{
					}
				}
			}
			//Check Collision with Self
			std::vector<std::shared_ptr<Bodies>> Quer = this->AlgoCheck->GetQueries(jj, B_Ex);
			for (auto& ii : Quer)
			{
				if (jj->GetID() != ii->GetID())
				{
					glm::vec3 Bod_Vel = Temp->GetVel();
					float F_dt = dt;
					if (this->BinColDetection(jj, ii,Bod_Vel,PrevPos,0,dt,F_dt))
					{
						std::shared_ptr<Manifold> T = this->Col_Rel->MakeManifold(jj, ii, -1);
						if (!this->ContainsManifold(ColRel, T))
							ColRel.push_back(T);
					}
				}
			}
		}
	}
	//Fix Resolution
	int Count = 0;
	for (auto& jj : ColRel)
	{
		//
		if (jj->ContactCount > 0)
		{
			float Diff = jj->Contacts[0]->Penetration/ 2.f;
			glm::vec3 Norm = jj->Contacts[0]->Normal;
			switch (jj->ID)
			{
			case 0:
				jj->Bod0->MovePosition( Diff * Norm);
				break;
			case 1:
				jj->Bod1->MovePosition(Diff * Norm);
				break;
			default:
				if (jj->Bod0->GetBodyParts()[0]->BodParticle)
				{
					jj->Bod0->GetBodyParts()[0]->BodParticle->ResetForce();
					jj->Bod0->MovePosition(Diff * Norm /2.f);
				}
				if (jj->Bod1->GetBodyParts()[0]->BodParticle)
				{
					jj->Bod1->GetBodyParts()[0]->BodParticle->ResetForce();
					jj->Bod1->MovePosition(Diff * -Norm /2.f);

				}
				break;
			}
		}
		Count++;

	}
	//Update All Physics
	for (auto& jj : AllBods)
	{
		std::shared_ptr<Particle> Temp = jj->GetSpecificBodyPart(0)->BodParticle;
		if (Temp)
		{
			//jj->SetPosition(Temp->UpdatePos(dt));			
		}
	}
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
