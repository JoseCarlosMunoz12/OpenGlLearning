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
	return S->GJK(Bod0->GetShapes(), Bod1->GetShapes());
}

bool BaseCols::ColBods(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1,
	std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1)
{
	return S->GJK(Bod0->GetShapes(), Bod1->GetShapes(), Seg0, Seg1);
}

BaseCols::BaseCols(std::string Name, std::shared_ptr<CollisionManager> InitCols)
{
	this->Name = Name;
	this->ColMan = InitCols;
	this->S = std::make_unique<GJK_Alg>();
	this->ColRel;
	this->ContCrt = std::make_unique<ContactCreation>();
}

BaseCols::~BaseCols()
{
}

std::vector<std::shared_ptr<Contact>> CoatlPhysicsEngine::BaseCols::GetColRel()
{
	return this->ColRel;
}

void BaseCols::SetTerrain(std::shared_ptr<Terrain> NewTer)
{
	this->Ter = NewTer;
}

std::vector<std::shared_ptr<Collection>> CoatlPhysicsEngine::BaseCols::GetCollections()
{
	return this->AllCollections;
}
