#include "Col_Resolution.h"
using namespace CoatlPhysicsEngine;
std::vector<std::shared_ptr<Contact>> Col_Resolution::MakeContacts(Sphere Sph0, std::shared_ptr<Bodies> Bod0)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod0->GetShapes()))
	{
		return this->S_Res->GetContacts(Sph0, *Sphere0);
	}
	else if (std::shared_ptr<AABB_Obj> Cube0 = std::dynamic_pointer_cast<AABB_Obj>(Bod0->GetShapes()))
	{
		return this->S_Res->GetContacts(Sph0,*Cube0);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod0->GetShapes()))
	{
		return this->S_Res->GetContacts(Sph0,*Cap0);
	}
	//else if (std::shared_ptr<Triangles> Tr = std::dynamic_pointer_cast<Triangles>(Bod0->GetShapes()))
	//{
	//	return this->S_Res->GetContacts(*Tr, Bod1);
	//}
	//else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(Bod0->GetShapes()))
	//{
	//	return this->S_Res->GetContacts(*Obj, Bod1);
	//}
	return std::vector<std::shared_ptr<Contact>>();

}

std::vector<std::shared_ptr<Contact>> Col_Resolution::MakeContacts(AABB_Obj Obj, std::shared_ptr<Bodies> Bod0)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod0->GetShapes()))
	{
		return this->S_Res->GetContacts(Obj, *Sphere0);
	}
	else if (std::shared_ptr<AABB_Obj> Cube0 = std::dynamic_pointer_cast<AABB_Obj>(Bod0->GetShapes()))
	{
		return this->S_Res->GetContacts(Obj, *Cube0);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod0->GetShapes()))
	{
		return this->S_Res->GetContacts(Obj, *Cap0);
	}
	//else if (std::shared_ptr<Triangles> Tr = std::dynamic_pointer_cast<Triangles>(Bod0->GetShapes()))
	//{
	//	return this->S_Res->GetContacts(*Tr, Bod1);
	//}
	//else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(Bod0->GetShapes()))
	//{
	//	return this->S_Res->GetContacts(*Obj, Bod1);
	//}
	return std::vector<std::shared_ptr<Contact>>();
}

std::vector<std::shared_ptr<Contact>> Col_Resolution::MakeContacts(Capsule Cap, std::shared_ptr<Bodies> Bod0)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod0->GetShapes()))
	{
		return this->S_Res->GetContacts(Cap, *Sphere0);
	}
	else if (std::shared_ptr<AABB_Obj> Cube0 = std::dynamic_pointer_cast<AABB_Obj>(Bod0->GetShapes()))
	{
		return this->S_Res->GetContacts(Cap, *Cube0);
	}
	//else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod0->GetShapes()))
	//{
	//	return this->S_Res->GetContacts(Obj, *Cap0);
	//}
	//else if (std::shared_ptr<Triangles> Tr = std::dynamic_pointer_cast<Triangles>(Bod0->GetShapes()))
	//{
	//	return this->S_Res->GetContacts(*Tr, Bod1);
	//}
	//else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(Bod0->GetShapes()))
	//{
	//	return this->S_Res->GetContacts(*Obj, Bod1);
	//}
	return std::vector<std::shared_ptr<Contact>>();
}

std::vector<std::shared_ptr<Contact>> Col_Resolution::MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod0->GetShapes()))
	{
		return this->MakeContacts(*Sphere0, Bod1);
	}
	else if (std::shared_ptr<AABB_Obj> Cube0 = std::dynamic_pointer_cast<AABB_Obj>(Bod0->GetShapes()))
	{
		return this->MakeContacts(*Cube0, Bod1);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod0->GetShapes()))
	{
		return this->MakeContacts(*Cap0, Bod1);
	}
	glm::vec3 Se;
	if (!this->S->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), Se))
	{
		float Dir = glm::sqrt(glm::dot(Se, Se));
		float Dirs = Dir;
	}
	else
	{

	}
	/*else if (std::shared_ptr<Triangles> Tr = std::dynamic_pointer_cast<Triangles>(Bod0->GetShapes()))
	{
		return this->MakeContacts(*Tr, Bod1);
	}
	else if (std::shared_ptr<OBB> Obj = std::dynamic_pointer_cast<OBB>(Bod0->GetShapes()))
	{
		return this->MakeContacts(*Obj, Bod1);
	}*/
	return std::vector<std::shared_ptr<Contact>>();
}

Col_Resolution::Col_Resolution()
{
	this->S_Res = std::make_unique<ShapeResolution>();
	this->S = std::make_unique<GJK_Alg>();
}

Col_Resolution::~Col_Resolution()
{
}

std::shared_ptr<Manifold> Col_Resolution::MakeManifold(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1, int ID)
{
	std::shared_ptr<Manifold> Temp = std::make_shared<Manifold>();
	Temp->Bod0 = Bod0;
	Temp->Bod1 = Bod1;
	Temp->Contacts = MakeContacts(Bod0, Bod1);
	Temp->ContactCount = Temp->Contacts.size();
	Temp->ID = ID;
	return Temp;
}
