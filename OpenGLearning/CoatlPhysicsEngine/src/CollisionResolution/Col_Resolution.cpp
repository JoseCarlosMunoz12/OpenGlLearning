#include "Col_Resolution.h"
using namespace CoatlPhysicsEngine;
std::vector<std::shared_ptr<Contact>> Col_Resolution::ContactCreate(Sphere Sph0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Sph0, *Sphere0);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Sph0, *Cap0);
	}
	else
	{
		glm::vec3 Vec;
		if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(),Vec))
		{
			float R = Sph0.GetRadius();
			float Pen = glm::distance(glm::vec3(0.f), Vec) - R;
			glm::vec3 Norm = glm::normalize(Vec);
		}
		else
		{
			glm::vec3 Norm;
			float Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(),Norm);
		}
	}
	return std::vector<std::shared_ptr<Contact>>();

}

std::vector<std::shared_ptr<Contact>> Col_Resolution::ContactCreate(Capsule Cap, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Cap, *Sphere0);
	}
	else if (std::shared_ptr<AABB_Obj> Cube0 = std::dynamic_pointer_cast<AABB_Obj>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Cap, *Cube0);
	}
	else
	{
		glm::vec3 NewDir;
		if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(),NewDir))
		{

		}
		else
		{
		}
	}
	return std::vector<std::shared_ptr<Contact>>();
}

std::vector<std::shared_ptr<Contact>> Col_Resolution::ContactCreate(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{

	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{

	}
	else
	{

	}
	return std::vector<std::shared_ptr<Contact>>();
}

std::vector<std::shared_ptr<Contact>> Col_Resolution::MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod0->GetShapes()))
	{
		return this->ContactCreate(*Sphere0, Bod0, Bod1);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod0->GetShapes()))
	{
		return this->ContactCreate(*Cap0, Bod0, Bod0);
	}
	return this->ContactCreate(Bod0, Bod1);	
}

Col_Resolution::Col_Resolution()
{
	this->S_Res = std::make_unique<ShapeResolution>();
	this->GJK_ = std::make_unique<GJK_Alg>();
	this->SAT_ = std::make_unique<SAT>();
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
