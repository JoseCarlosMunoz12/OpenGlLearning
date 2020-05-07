#include "BaseCols.h"
using namespace CoatlPhysicsEngine;

bool BaseCols::UpdateBodies(Sphere Bod0, std::shared_ptr<Bodies> Bod1)
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

bool BaseCols::UpdateBodies(AABB_Obj Obj0, std::shared_ptr<Bodies> Bod1)
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

bool BaseCols::UpdateBodies(Capsule Cap0, std::shared_ptr<Bodies> Bod1)
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

bool BaseCols::UpdateBodies(Triangles Tr, std::shared_ptr<Bodies> Bod1)
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

bool BaseCols::UpdateBodies(OBB Tr, std::shared_ptr<Bodies> Bod1)
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

void BaseCols::ColBods(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
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

BaseCols::BaseCols(std::string Name, std::shared_ptr<CollisionManager> InitCols)
{
	this->Name = Name;
	this->ColMan = InitCols;
}

BaseCols::~BaseCols()
{
}

void BaseCols::SetTerrain(std::shared_ptr<Terrain> NewTer)
{
	this->Ter = NewTer;
}
