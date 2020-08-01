#include "BaseCols.h"
using namespace CoatlPhysicsEngine;

bool BaseCols::UpdateBodies(Sphere Sph0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{	
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Sph0, *Sphere0);
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Sph0, *Cap);
	}
	return S->GJK( Bod1->GetShapes(),Bod0->GetShapes());
}

bool BaseCols::UpdateBodies(Capsule Cap0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Cap0, *Sphere0);
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->ColMan->CheckCollide(Cap0, *Cap);
	}
	return S->GJK( Bod1->GetShapes(),Bod0->GetShapes());
}

bool BaseCols::ColBods(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod0->GetShapes()))
	{
		return this->UpdateBodies(*Sphere0,Bod0, Bod1);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod0->GetShapes()))
	{
		return this->UpdateBodies(*Cap0,Bod0, Bod1);
	}
	return S->GJK(Bod0->GetShapes(), Bod1->GetShapes());
}

BaseCols::BaseCols(std::string Name, std::shared_ptr<CollisionManager> InitCols)
{
	this->Name = Name;
	this->ColMan = InitCols;
	this->S = std::make_unique<GJK_Alg>();
	this->ColRel;
}

BaseCols::~BaseCols()
{
}

void BaseCols::SetTerrain(std::shared_ptr<Terrain> NewTer)
{
	this->Ter = NewTer;
}
