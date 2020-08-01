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
	if (!this->GJK_->EPA_GJK( Bod0->GetShapes(),Bod1->GetShapes(),Vec))
	{
		float R = Sph0.GetRadius();
		float Pen = R - glm::distance(glm::vec3(0.f), Vec);
		glm::vec3 Norm = glm::normalize(Vec);
		Cont->Penetration = Pen;
		Cont->Normal = Norm;
		Cont->ContactPoint = Sph0.GetPos() - R * Norm;
		Temp.push_back(Cont);
	}
	else
	{
		glm::vec3 Norm;
		float R = Sph0.GetRadius();
		float Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(),Norm) + R;
		Cont->Penetration = Pen;
		Cont->Normal = -Norm;
		Cont->ContactPoint = Sph0.GetPos() - Pen * Norm;
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
	glm::vec3 vec;
	if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), vec))
	{
		float R = Cap.GetRadius();
		std::vector<glm::vec3> Cap_Seg = Cap.GetSegment();
		std::vector<glm::vec3> Obj_seg = Bod1->GetShapes()->GetVertices();
		std::vector<glm::vec3> Obj_Norm = Bod1->GetShapes()->GetNormals();
		glm::vec3 Dir = MATH::CreateNormal(Cap_Seg);
		float Pen = glm::distance(glm::vec3(0.f),vec);
		glm::vec3 Norm = MATH::Normalize(vec);
		MATH::SAT_Point_Cul(Norm, Cap_Seg, Obj_seg);
		for (auto& jj : Obj_Norm)
		{
			glm::vec3 Dot = glm::cross(jj, Norm);
			if (Dot != glm::vec3(0.f))
				MATH::SAT_Clip(jj, Cap_Seg, Obj_seg);
		}
		for (auto& jj : Cap_Seg)
		{
			int Count = 0;
			for (auto& ii : Obj_seg)
			{
				glm::vec3 RelNorm = MATH::Normalize(jj - ii);
				if (RelNorm == Norm)
				{
					std::shared_ptr<Contact> Cont = std::make_shared<Contact>();
					Cont->Normal = Norm;
					Cont->Penetration = R - Pen;
					Cont->ContactPoint = (jj + ii) / 2.f;
					Temp.push_back(Cont);
					break;
				}
				Count++;
			}
			Obj_seg.erase(Obj_seg.begin() + Count);
		}
	}
	else
	{
		std::shared_ptr<Contact> Cont = std::make_shared<Contact>();
		glm::vec3 Norm;
		float R = Cap.GetRadius();
		float Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(), Norm) + R;
		Cont->Penetration = Pen;
		Cont->Normal = Norm;
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
		return this->ContactCreate(*Sph, Bod1, Bod0);
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->ContactCreate(*Cap, Bod1, Bod0);
	}
	else
	{
		this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), Vec);
		float Pen = glm::distance(Vec, glm::vec3(0.f));		
		glm::vec3 Norm = MATH::Normalize(Vec);
		std::vector<glm::vec3> Obj0_seg = Bod0->GetShapes()->GetVertices();
		std::vector<glm::vec3> Obj1_seg = Bod1->GetShapes()->GetVertices();
		std::vector<glm::vec3> Obj1_Norm = Bod1->GetShapes()->GetNormals();
		MATH::SAT_Point_Cul(Norm,Obj0_seg, Obj1_seg);
		for (auto& ii : Obj1_Norm)
		{
			glm::vec3 Dot = glm::cross(ii, Norm);
			if (Dot != glm::vec3(0.f))
				MATH::SAT_Clip(ii, Obj0_seg, Obj1_seg);
		}
		for (auto& jj : Obj0_seg)
		{
			for (auto& ii : Obj1_seg)
			{
				glm::vec3 RelNorm = glm::cross(MATH::Normalize(ii - jj),Norm);
				if (RelNorm == glm::vec3(0.f))
				{
					std::shared_ptr<Contact> Cont = std::make_shared<Contact>();
					Cont->Normal = Norm;
					Cont->Penetration = Pen;
					Cont->ContactPoint = (jj + ii) / 2.f;
					Temp.push_back(Cont);
					break;
				}
			}
		}
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

void Col_Resolution::ResolveContacts(std::shared_ptr<Manifold> Contact)
{

}
