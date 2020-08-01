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

bool DynamicCollisions::BinColDetection(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1,
	glm::vec3 Vel0, glm::vec3 Vel1,
	glm::vec3 InitPos0, glm::vec3 InitPos1,
	float t0, float t1, float& NewDt)
{
	if ((t1 - t0) < EPSILON)
	{
		NewDt = t0;
		return true;
	}
	float Mid = t0 + (t1 - t0) / 2.f;
	glm::vec3 TempPos0;
	glm::vec3 TempPos1;
	MATH::ClosestSeg_Seg({ InitPos0 + Vel0 * t0,InitPos0 + Vel0 * t1 },
		{ InitPos1 + Vel1 * t0,InitPos1 + Vel1 * t1 },TempPos0,TempPos1);
	Bod0->SetPosition(TempPos0);
	float Dis = glm::distance(InitPos0 + Vel0 * t0, InitPos0 + Vel0 * t1);
	float Contdis = glm::distance(InitPos0 + Vel0 * t0,TempPos0 );
	float Loc  = 0.f;
	if (Dis != 0.f)
		Loc =  Contdis/Dis  * (t1 - t0);
	Bod1->SetPosition(InitPos1 + (t0 + Loc) * Vel1);
	if (!this->ColBods(Bod0, Bod1))
	{
		return false;
	}
	if (BinColDetection(Bod0, Bod1, Vel0,Vel1, InitPos0,InitPos1, t0, Mid, NewDt))
	{
		return true;
	}
	return BinColDetection(Bod0, Bod1, Vel0, Vel1, InitPos0, InitPos1, Mid, t1, NewDt);
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
	Ext(100.f), AlgoType(Alg_Type::O_T), B_Ex(4.f)
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
	this->ColRel.clear();
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
		std::shared_ptr<Bod_Base> Temp = jj->GetParticle(0);
		if (Temp)
		{
			//resetForces on the Object///////////////////////////////////////////////////////////
			Temp->ResetForce();
			//Gravitaional Force
			Temp->AcumForce(this->Grav_F_Manager->GetForce(*Temp));
			// Drag Force
			/*this->F_Manager = std::make_unique<Phy_Bungee>(glm::vec3(0.f,0.f,10.f),1000,100,5);
			Temp->AcumForce(this->F_Manager->GetForce(*Temp));*/
			// Spring Force
			this->F_Manager = std::make_unique<Phy_Drag>(1, 0);
			Temp->AcumForce(this->F_Manager->GetForce(*Temp));
			glm::vec3 PrevPos = jj->GetPos();
			glm::vec3 Bod_Vel= Temp->GetVel();
			float F_dt = dt;
			//Check Collision with The Terrain////////////////////////////////////////////////////
			if (!Ter.expired())
			{
				std::vector<std::shared_ptr<Bodies>> Quers = Ter.lock()->GetTerrs(jj->GetPos(), 1);
				if (!this->Ter.expired())
				{					
					for (auto& ii : Quers)
					{
						if (this->BinColDetection(jj, ii,Bod_Vel, PrevPos, 0, dt, F_dt))
						{
							jj->MovePosition(F_dt * Bod_Vel);
							std::shared_ptr<Manifold> T = this->Col_Rel->MakeManifold(jj, ii, 0);
							if (!this->ContainsManifold(ColRel, T))
								ColRel.push_back(T);
							Temp->AcumForce(-Gravity * Temp->GetMass());
							if (Bod_Vel.z < 0)
							{
								if (glm::abs(Bod_Vel.z) > 0.125f)
								{
									Temp->SetVel(glm::vec3(Bod_Vel.x, Bod_Vel.y,glm::abs(Bod_Vel.z/2)));
								}
								else
								{
									Temp->SetVel(glm::vec3(Bod_Vel.x, Bod_Vel.y, 0.f));
								}
							}
							jj->SetPosition(PrevPos);
						}
					}
				}
			}
			//Check Collision with Static Bodies//////////////////////////////////////////////////
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
						Temp->AcumForce(-Gravity * Temp->GetMass());
						if (glm::abs(Bod_Vel.z) > 0.0625f)
						{
							Temp->SetVel(glm::vec3(Bod_Vel.x, Bod_Vel.y, -Bod_Vel.z / 2));
						}
						else
						{
							Temp->SetVel(glm::vec3(Bod_Vel.x, Bod_Vel.y, 0.f));
						}
						jj->SetPosition(PrevPos);
					}
				}
			}
			//Check Collision with othe Dynamics//////////////////////////////////////////////////
			std::vector<std::shared_ptr<Bodies>> Quer = this->AlgoCheck->GetQueries(jj, B_Ex);
			for (auto& ii : Quer)
			{
				if (ii->GetParticle(0))
				{
					/*this->F_Manager = std::make_unique<Phy_Spring>(10, 20);
					glm::vec3 F_S = this->F_Manager->GetForce(*ii->GetParticle(0), *jj->GetParticle(0));
					ii->GetParticle(0)->AcumForce(F_S);
					jj->GetParticle(0)->AcumForce(-F_S);*/
					if (jj->GetID() != ii->GetID())
					{
						glm::vec3 Bod1_Vel = ii->GetParticle(0)->GetVel();
						glm::vec3 Pos1 = ii->GetPos();
						float F_dt = dt;
						if (this->BinColDetection(jj, ii,Bod_Vel,Bod1_Vel,PrevPos,Pos1,0,dt,F_dt))
						{
							std::shared_ptr<Manifold> T = this->Col_Rel->MakeManifold(jj, ii, -1);
							if (!this->ContainsManifold(ColRel, T))
								ColRel.push_back(T);
							ii->SetPosition(Pos1);
							jj->SetPosition(PrevPos);
						}
					}
				}
			}
		}
	}
	//Fix Resolution
	for (auto& jj : ColRel)
	{
		//(0.f;false
		if (jj->ContactCount > 0)
		{
			float Diff = jj->Contacts[0]->Penetration;
			if (Diff < 0.001)
				Diff = 0.f;
			glm::vec3 Norm = jj->Contacts[0]->Normal;
			switch (jj->ID)
			{
			case 0:
				jj->Bod0->MovePosition(Diff * Norm);
				break;
			case 1:
				jj->Bod1->MovePosition(Diff * Norm);
				break;
			default:
				if (jj->Bod0->GetBodyParts()[0]->GetParticle())
				{
					jj->Bod0->MovePosition(Diff * Norm /2.f);
				}
				if (jj->Bod1->GetBodyParts()[0]->GetParticle())
				{
					jj->Bod1->MovePosition(-Diff * Norm /2.f);
				}
				break;
			}
		}
		this->Col_Rel->ResolveContacts(jj);
	}
	//Update All Physics	
	for (auto& jj : AllBods)
	{
		std::shared_ptr<Bod_Base> Temp = jj->GetSpecificBodyPart(0)->GetParticle();
		if (Temp)
		{
		//	jj->SetPosition(Temp->UpdatePos(dt));
			jj->UpdateAABB();
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
