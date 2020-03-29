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
