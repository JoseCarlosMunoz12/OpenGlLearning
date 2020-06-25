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
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cont = std::make_shared<Contact>();
	glm::vec3 Vec;
	if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(),Vec))
	{
		float R = Sph0.GetRadius();
		float Pen = R - glm::distance(glm::vec3(0.f), Vec);
		glm::vec3 Norm = glm::normalize(Vec);
		Cont->Penetration = Pen;
		Cont->Normal = -Norm;
		Cont->ContactPoint = Sph0.GetPos() + Pen * Norm;
		Temp.push_back(Cont);
	}
	else
	{
		glm::vec3 Norm;
		float R = Sph0.GetRadius();
		float Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(),Norm) + R;
		Cont->Penetration = Pen + 0.001f;
		Cont->Normal = -Norm;
		Cont->ContactPoint = Sph0.GetPos() + Pen * Norm;
		Temp.push_back(Cont);
	}
	return Temp;
}

std::vector<std::shared_ptr<Contact>> Col_Resolution::ContactCreate(Capsule Cap, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Cap, *Sphere0);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Cap, *Cap0);
	}
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cont = std::make_shared<Contact>();
	glm::vec3 Vec;
	if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), Vec))
	{
		float R = Cap.GetRadius();
		float Pen = glm::distance(glm::vec3(0.f), Vec);
		glm::vec3 Norm =  glm::normalize(Vec);
		Cont->Penetration = R - Pen;
		Cont->Normal = -Norm;
		Cont->ContactPoint = Cap.GetPos() - Pen * Norm;
		Temp.push_back(Cont);
	}
	else
	{
		glm::vec3 Norm;
		float R = Cap.GetRadius();
		float Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(), Norm) + R;
		Cont->Penetration = Pen;
		Cont->Normal = -Norm;
		Cont->ContactPoint = Cap.GetPos() + Pen * Norm;
		Temp.push_back(Cont);
	}
	return Temp;
}

std::vector<std::shared_ptr<Contact>> Col_Resolution::ContactCreate(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cont = std::make_shared<Contact>();
	glm::vec3 Vec;
	if (std::shared_ptr<Sphere> Sph = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{

		if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), Vec))
		{
			float R = Sph->GetRadius();
			float Pen = R - glm::distance(glm::vec3(0.f), Vec);
			glm::vec3 Norm = glm::normalize(Vec);
			Cont->Penetration = Pen - 0.001f;
			Cont->Normal = -Norm;
			Cont->ContactPoint = Sph->GetPos() + Pen * Norm;
			Temp.push_back(Cont);
		}
		else
		{
			glm::vec3 Norm;
			float R = Sph->GetRadius();
			float Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(), Norm) + R;
			Cont->Penetration = Pen - 0.001f;
			Cont->Normal = -Norm;
			Cont->ContactPoint = Sph->GetPos() + Pen * Norm;
			Temp.push_back(Cont);
		}
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), Vec))
		{
			float R = Cap->GetRadius();
			float Pen = R - glm::distance(glm::vec3(0.f), Vec);
			glm::vec3 Norm = glm::normalize(Vec);
			Cont->Penetration = Pen - 0.001f;
			Cont->Normal = -Norm;
			Cont->ContactPoint = Cap->GetPos() + Pen * Norm;
			Temp.push_back(Cont);
		}
		else
		{
			glm::vec3 Norm;
			float R = Cap->GetRadius();
			float Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(), Norm) + R;
			Cont->Penetration = Pen - 0.001f;
			Cont->Normal = -Norm;
			Cont->ContactPoint = Cap->GetPos() + Pen * Norm;
			Temp.push_back(Cont);
		}
	}
	else
	{
		float Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(), Vec);
		Cont->Penetration = Pen;
		Cont->Normal = -Vec;
		Cont->ContactPoint = Bod0->GetShapes()->GetPos() + Pen * Vec;
		Temp.push_back(Cont);
	}
	return Temp;
}

std::vector<std::shared_ptr<Contact>> Col_Resolution::MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod0->GetShapes()))
	{
		return this->ContactCreate(*Sphere0, Bod0, Bod1);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod0->GetShapes()))
	{
		return this->ContactCreate(*Cap0, Bod0, Bod1);
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
