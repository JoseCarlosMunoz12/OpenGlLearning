#include "StaticCollisions.h"
using namespace CoatlPhysicsEngine;

bool StaticCollisions::UpdateBodies(Sphere Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Bod0, *Sphere0);
	}
	else if (std::shared_ptr<AABB_Obj> AABB = std::dynamic_pointer_cast<AABB_Obj>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Bod0, *AABB);
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Bod0, *Cap);
	}
	else if (std::shared_ptr<Triangles> Tr = std::dynamic_pointer_cast<Triangles>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Bod0, *Tr);
	}
	else if (std::shared_ptr<OBB> Tr = std::dynamic_pointer_cast<OBB>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Bod0, *Tr);
	}
	return false;	
}

bool StaticCollisions::UpdateBodies(AABB_Obj Obj0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Obj0, *Sphere0);
	}
	else if (std::shared_ptr<AABB_Obj> AABB = std::dynamic_pointer_cast<AABB_Obj>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Obj0, *AABB);
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Obj0, *Cap);
	}
	else if (std::shared_ptr<Triangles> Tr = std::dynamic_pointer_cast<Triangles>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Obj0, *Tr);
	}
	else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Obj0, *Obj);
	}
	return false;
}

bool StaticCollisions::UpdateBodies(Capsule Cap0, std::shared_ptr<Bodies> Bod1)
{

	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Cap0, *Sphere0);
	}
	else if (std::shared_ptr<AABB_Obj> AABB = std::dynamic_pointer_cast<AABB_Obj>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Cap0, *AABB);
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Cap0, *Cap);
	}
	else if (std::shared_ptr<Triangles> Tr = std::dynamic_pointer_cast<Triangles>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Cap0, *Tr);
	}
	else if (std::shared_ptr<OBB> Tr = std::dynamic_pointer_cast<OBB>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Cap0, *Tr);
	}
	return false;
}

bool StaticCollisions::UpdateBodies(Triangles Tr, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *Sphere0);
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *Cap);
	}
	else if (std::shared_ptr<AABB_Obj> AABB = std::dynamic_pointer_cast<AABB_Obj>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *AABB);
	}
	else if (std::shared_ptr<Triangles> Tr1 = std::dynamic_pointer_cast<Triangles>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *Tr1);
	}
	else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *Obj);
	}
	return false;
}

bool StaticCollisions::UpdateBodies(OBB Tr, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *Sphere0);
	}
	else if (std::shared_ptr<AABB_Obj> AABB = std::dynamic_pointer_cast<AABB_Obj>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *AABB);
	}
	else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *Obj);
	}
	else if (std::shared_ptr<Triangles> Tr1 = std::dynamic_pointer_cast<Triangles>(Bod1->GetShapes()))
	{
			return this->ColMan->CheckCollide(Tr, *Tr1);
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *Cap);
	}
	return false;
}

void StaticCollisions::ColBods(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	bool Check = false;
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod0->GetShapes()))
	{
		Check = this->UpdateBodies(*Sphere0, Bod1);
	}
	else if (std::shared_ptr<AABB_Obj> Cube0 = std::dynamic_pointer_cast<AABB_Obj>(Bod0->GetShapes()))
	{
		Check = this->UpdateBodies(*Cube0, Bod1);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod0->GetShapes()))
	{
		Check = this->UpdateBodies(*Cap0, Bod1);
	}
	else if (std::shared_ptr<Triangles> Tr = std::dynamic_pointer_cast<Triangles>(Bod0->GetShapes()))
	{
		Check = this->UpdateBodies(*Tr, Bod1);
	}
	else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(Bod0->GetShapes()))
	{
		Check = this->UpdateBodies(*Obj, Bod1);
	}
	Bod0->UpDateBodiesInf(Check, Bod1);
	Bod1->UpDateBodiesInf(Check, Bod0);
}

StaticCollisions::StaticCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols)
	:Ext(100.f),AlgoType(Alg_Type::B_F),B_Ex(4.f)
{
	this->ColMan = InitCols;
	this->Name = Name;	
}

StaticCollisions::~StaticCollisions()
{	
}

void StaticCollisions::UpdateCollisionCheck()
{
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
	for (auto& jj : AllStatics)
	{
		this->AlgoCheck->Insert(jj);
	}
	//get queries and test them
	int Size = AllStatics.size();
	//O(nlog) for Quadtree and OctoTree
	//O(n^2) for B_Force
	for (int ii = 0; ii < Size; ii++)
	{
		std::vector<std::shared_ptr<Bodies>> Quer = this->AlgoCheck->GetQueries(AllStatics[ii], Ext);
		for (auto& kk : Quer)
		{
			this->ColBods(AllStatics[ii], kk);
		}
	}
}

void StaticCollisions::AddNewBody( std::shared_ptr<ColShapes> NewShape)
{
	this->AllStatics.push_back(std::make_shared<Bodies>(NewShape,this->NewCurId));
	this->NewCurId++;
}

void StaticCollisions::AddNewBody(std::vector<std::shared_ptr<ColShapes>> NewShapes)
{
	for (auto& ii : NewShapes)
	{
		this->AllStatics.push_back(std::make_shared<Bodies>(ii, this->NewCurId));
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
	for (auto& jj : this->AllStatics)
	{
		Temp.push_back(jj);
	}
	return Temp;
}

std::shared_ptr<Bodies> StaticCollisions::GetABody(int ID)
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

Alg_Type StaticCollisions::GetType()
{
	return this->AlgoType;
}

void StaticCollisions::SetNewType(Alg_Type NewType)
{
	this->AlgoType = NewType;
}
