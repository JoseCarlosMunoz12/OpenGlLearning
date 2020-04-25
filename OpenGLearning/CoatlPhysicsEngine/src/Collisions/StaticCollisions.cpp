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
	return false;
}

bool StaticCollisions::UpdateBodies(OBB Tr, std::shared_ptr<Bodies> Bod1)
{
	//if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	//{
	//	return this->ColMan->CheckCollide(Tr, *Sphere0);
	//}
	//else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	//{
	//	return this->ColMan->CheckCollide(Tr, *Cap);
	//}
	//else 
		if (std::shared_ptr<AABB_Obj> AABB = std::dynamic_pointer_cast<AABB_Obj>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *AABB);
	}
	else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Tr, *Obj);
	}
	return false;
}

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
	bool Check = false;
	for (int ii = 0; ii < Size; ii++)
	{
		for (int jj = ii + 1; jj < Size; jj++)
		{
			if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(AllStatics[ii]->GetShapes()))
			{
				 Check = this->UpdateBodies(*Sphere0, AllStatics[jj]);				
			}
			else if (std::shared_ptr<AABB_Obj> Cube0 = std::dynamic_pointer_cast<AABB_Obj>(AllStatics[ii]->GetShapes()))
			{
				Check = this->UpdateBodies(*Cube0, AllStatics[jj]);
			}
			else if ( std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(AllStatics[ii]->GetShapes()))
			{
				Check = this->UpdateBodies(*Cap0,AllStatics[jj]);
			}
			else if (std::shared_ptr<Triangles> Tr = std::dynamic_pointer_cast<Triangles>(AllStatics[ii]->GetShapes()))
			{
				Check = this->UpdateBodies(*Tr, AllStatics[jj]);
			}
			else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(AllStatics[ii]->GetShapes()))
			{
				Check = this->UpdateBodies(*Obj, AllStatics[jj]);
			}
			AllStatics[ii]->UpDateBodiesInf(Check, AllStatics[jj]);
			AllStatics[jj]->UpDateBodiesInf(Check, AllStatics[ii]);
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

std::vector<std::weak_ptr<Bodies>> CoatlPhysicsEngine::StaticCollisions::GetAllBodies()
{
	std::vector<std::weak_ptr<Bodies>> Temp;
	for (auto& jj : this->AllStatics)
	{
		Temp.push_back(jj);
	}
	return Temp;
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
