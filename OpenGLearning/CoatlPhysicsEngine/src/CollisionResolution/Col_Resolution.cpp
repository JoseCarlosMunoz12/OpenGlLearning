#include "Col_Resolution.h"
using namespace CoatlPhysicsEngine;

void Col_Resolution::PrepeareContact(std::shared_ptr<Manifold> Cnt, float dt)
{
	for (auto& jj : Cnt->Contacts)
		jj->CalculateInternals(Cnt->Bod0, Cnt->Bod1, dt);
}

void Col_Resolution::AdjustPosition(std::shared_ptr<Manifold> Cnt, float dt)
{

}

void Col_Resolution::AdjustVelocity(std::shared_ptr<Manifold> Cnt, float dt)
{

}

void Col_Resolution::ResolveResolution(std::shared_ptr<Bodies> Bod, std::shared_ptr<Manifold> Cnt)
{
	glm::vec3 Norm = Cnt->Contacts[0]->Normal;
	float Diff = Cnt->Contacts[0]->Penetration;
	if (Diff < 0.001)
		Diff = 0.f;
	Bod->MovePosition(Diff * Norm);
}

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
	float Dis;
	if (!this->GJK_->EPA_GJK( Bod0->GetShapes(),Bod1->GetShapes(),Vec, Dis))
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
	float Pen;
	if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), vec,Pen))
	{
		float R = Cap.GetRadius();
		std::vector<glm::vec3> Cap_Seg = Cap.GetSegment();
		std::vector<glm::vec3> Obj_seg = Bod1->GetShapes()->GetVertices();
		std::vector<glm::vec3> Obj_Norm = Bod1->GetShapes()->GetNormals();
		glm::vec3 Dir = MATH::CreateNormal(Cap_Seg);
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
			}
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
	if (std::shared_ptr<Sphere> Sph = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->ContactCreate(*Sph, Bod1, Bod0);
	}
	else if (std::shared_ptr<Capsule> Cap = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->ContactCreate(*Cap, Bod1, Bod0);
	}
	float Pen = 0.f;
	glm::vec3 Norm = glm::vec3(0.f);
	//Uses GJK to get penetration and Direction
	this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), Norm,Pen);
	//uses SAT to get contact points in one shot
	return this->SAT_->SAT_CreateContacts(Bod0->GetShapes(), Bod1->GetShapes(),
		Norm, Pen);
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

void Col_Resolution::MakeJointManifold(std::vector<std::shared_ptr<Manifold>> &R,std::vector<std::shared_ptr<ColJoints>> BodJoints)
{
	std::shared_ptr<Manifold> Temp = std::make_shared<Manifold>();
	for (auto& ii : BodJoints)
		ii->GetBodId();
}

void Col_Resolution::ResolveContacts(std::shared_ptr<Manifold> Cnt,float dt)
{
	//(false;Cnt->Contacts[0]->Normal; Cnt->Contacts[0]->Penetration;
	if (Cnt->ContactCount > 0)
	{
		float Diff = Cnt->Contacts[0]->Penetration;
		if (Diff < 0.001)
			Diff = 0.f;
		glm::vec3 Norm = Cnt->Contacts[0]->Normal;
		switch (Cnt->ID)
		{
		case 0:
			this->ResolveResolution(Cnt->Bod0, Cnt);
			break;
		case 1:
			this->ResolveResolution(Cnt->Bod1, Cnt);
			break;
		case 2:
			break;
		default:
			if (Cnt->Bod0->GetBodyParts()->GetParticle())
			{
				Cnt->Bod0->MovePosition(Diff * Norm / 2.f);
			}
			if (Cnt->Bod1->GetBodyParts()->GetParticle())
			{
				Cnt->Bod1->MovePosition(-Diff * Norm / 2.f);
			}
			break;
		}
	}
	//Prepare bodies for resolution
	this->PrepeareContact(Cnt, dt);
	//Adjust the positions
	this->AdjustPosition(Cnt, dt);
	//adjust the velocities of the bodies
	this->AdjustVelocity(Cnt, dt);
}
