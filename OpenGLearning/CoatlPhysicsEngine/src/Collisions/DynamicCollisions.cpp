#include "DynamicCollisions.h"
using namespace CoatlPhysicsEngine;

bool DynamicCollisions::BinColDetection(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1,
	glm::vec3 Vel0, glm::vec3 Vel1,
	float t0, float t1, float& NewDt)
{
	if ((t1 - t0) < EPSILON)
	{
		NewDt = t0;
		return true;
	}
	float Mid = t0 + (t1 - t0) / 2.f;
	if (!this->ColBods(Bod0, Bod1, {Vel0 * t0, Vel0 * t1}, {Vel1 * t0, Vel1 * t1}))
	{
		return false;
	}
	if (BinColDetection(Bod0, Bod1, Vel0,Vel1, t0, Mid, NewDt))
	{
		return true;
	}
	return BinColDetection(Bod0, Bod1, Vel0, Vel1, Mid, t1, NewDt);
}

bool DynamicCollisions::ContainsManifold(std::vector<std::shared_ptr<Contacts>> ColRel, std::shared_ptr<Contacts> NewMan)
{
	for (auto& jj : ColRel)
	{
		bool F_Same = false;
		bool S_Same = false;
		if (jj->Bods[0]->GetID() == NewMan->Bods[0]->GetID() || jj->Bods[1]->GetID() == NewMan->Bods[0]->GetID())
			F_Same = true;
		if (jj->Bods[0]->GetID() == NewMan->Bods[1]->GetID() || jj->Bods[1]->GetID() == NewMan->Bods[1]->GetID())
			S_Same = true;
		if (S_Same && F_Same)
			return true;
	}
	return false;
}

void DynamicCollisions::CullManifolds(std::vector<std::shared_ptr<Contacts>>& Cnt )
{
}

DynamicCollisions::DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
	:BaseCols(Name,InitCols),
	Ext(100.f), AlgoType(Alg_Type::O_T), B_Ex(4.f)
{
	this->Col_Rel = std::make_unique<Col_Resolution>(40,40);
	this->Gravity = glm::vec3(0.f, 0.f, -9.81f);
	this->Grav_F_Manager = std::make_unique<Phy_Grav>(this->Gravity);
}

DynamicCollisions::~DynamicCollisions()
{

}

void DynamicCollisions::CheckCollision(std::shared_ptr<StaticCollisions> Statics, std::shared_ptr<KinematicsCollisions> Kin, float dt)
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
		jj->ResetBools();
	}
	//////-----------Physics Loop-----------------\\\\\\\\\\
	//1) Force generators
	//2) Calc Physics
	//3) Check Collision
	//4) Resolve Manifolds
	////-------------------------------------------------\\\\
	//Update All Physics	
	for (auto& jj : AllBods)
	{
		std::shared_ptr<Bod_Base> Temp = jj->GetBodyParts()->GetParticle();
		if (Temp)
		{
			Temp->UpdatePos(dt);
			jj->UpdateAABB();
		}
	}
	//Find all Collisions and Calculate its Force Generators
	for (auto& jj : AllBods)
	{
		std::shared_ptr<Bod_Base> Temp = jj->GetParticle();
		if (Temp)
		{
			//resetForces on the Object///////////////////////////////////////////////////////////
			Temp->ResetForce();
			//Temp->AcumForce(this->Grav_F_Manager->GetForce(*Temp));
			glm::vec3 PrevPos = jj->GetPos();
			glm::quat PrevQuat = jj->GetQuat();
			glm::vec3 Bod_Vel= Temp->GetVel();
			glm::vec3 Bod_RotVel = Temp->GetRotVel();
			float F_dt = dt;
			//Check Collision with The Terrain////////////////////////////////////////////////////
			if (!this->Ter.expired())
			{
				std::vector<std::shared_ptr<Bodies>> Quers = Ter.lock()->GetTerrs(jj->GetPos(), 1);
				for (auto& ii : Quers)
				{
					if (this->BinColDetection(jj, ii,Bod_Vel,glm::vec3(0.f),0.f, dt, F_dt))
					{
						jj->MovePosition(F_dt * Bod_Vel);
						std::vector <std::shared_ptr<Contacts>> T = this->ContCrt->MakeManifold(jj, ii, 0);
						if(T.size() != 0)
							if (!this->ContainsManifold(ColRel, T[0]))
								for(auto& pp : T)
									ColRel.push_back(pp);
						//Temp->AcumForce(-Gravity * Temp->GetMass());							
						jj->SetPosition(PrevPos);
						//this->it = false;
					}
					else
						F_dt = dt;
				}
			}
			//Check Collision with Static Bodies//////////////////////////////////////////////////
			if (Statics)
			{
				std::vector<std::shared_ptr<Bodies>> Que = Statics->GetBods(jj);
				for (auto& ii : Que)
				{
					if (this->BinColDetection(jj, ii, Bod_Vel,glm::vec3(0.f), 0.f, dt, F_dt))
					{
						std::vector <std::shared_ptr<Contacts>> T = this->ContCrt->MakeManifold(jj, ii, 0);
						if (T.size() != 0)
							if (!this->ContainsManifold(ColRel, T[0]))
								for(auto& pp : T)
									ColRel.push_back(pp);
						//Temp->AcumForce(-Gravity * Temp->GetMass());
						jj->SetPosition(PrevPos);
					}
				}
			}
			//Check Collision with Kinematic Bodies///////////////////////////////////////////////
			if (Kin)
			{
				std::vector<std::shared_ptr<Bodies>> Que = Kin->GetBods(jj);
				for (auto& ii : Que)
				{
					glm::vec3 KinVel = ii->GetParticle()->GetVel();
					glm::vec3 KinPos = ii->GetPos();
					if (this->BinColDetection(jj, ii, Bod_Vel, KinVel, 0.f, dt, F_dt))
					{
						jj->MovePosition(F_dt * Bod_Vel);
						ii->MovePosition(F_dt* KinVel);
						std::vector <std::shared_ptr<Contacts>> T = this->ContCrt->MakeManifold(jj, ii, 0);
						if (T.size() != 0)
							if (!this->ContainsManifold(ColRel, T[0]))
								for (auto& pp : T)
									ColRel.push_back(pp);
						ii->SetPosition(KinPos);
						jj->SetPosition(PrevPos);
					}
				}
			}
			//Check Collision with othe Dynamics//////////////////////////////////////////////////
			std::vector<std::shared_ptr<Bodies>> Quer = this->AlgoCheck->GetQueries(jj, B_Ex);
			for (auto& ii : Quer)
			{
				if (ii->GetParticle())
				{
					if (jj->GetID() != ii->GetID())
					{
						//Get other body vel, pos, Rot and RotVel
						glm::vec3 Bod1_Vel = ii->GetParticle()->GetVel();
						glm::vec3 Pos1 = ii->GetPos();
						float F_dt = dt;
						if(!jj->HasId(ii))
						if (this->BinColDetection(jj, ii,Bod_Vel,Bod1_Vel,0,dt,F_dt))
						{
							jj->AddId(ii->GetID());
							ii->AddId(jj->GetID());
							jj->MovePosition(Bod_Vel * F_dt);
							ii->MovePosition(Bod1_Vel * F_dt);
							std::vector <std::shared_ptr<Contacts>> T = this->ContCrt->MakeManifold(jj, ii, 0);
							if(T.size() !=0)
								if (!this->ContainsManifold(ColRel, T[0]))
									for (auto& pp : T)
										ColRel.push_back(pp);
							ii->SetPosition(Pos1);
							jj->SetPosition(PrevPos);
							//this->it = false;
						}
					}
				}
			}
			//Updated the position of the current body
			jj->MovePosition(F_dt * Bod_Vel);
		}
	}
	//Get Manifold from Joints
	for (auto& jj : AllCollections)
		this->ContCrt->MakeJointManifold(this->ColRel, jj->GetJoints());
	//Cull Manifolds and get rid of repeats

	//Fix Resolution
	this->Col_Rel->ResolveContacts(this->ColRel, dt);
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
