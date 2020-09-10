#include "ContactCreation.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contacts>> ContactCreation::ContactCreate(Sphere Sph0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Sph0, *Sphere0);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Sph0, *Cap0);
	}
	std::vector<std::shared_ptr<Contacts>> Temp;
	std::shared_ptr<Contacts> Cont = std::make_shared<Contacts>();
	glm::vec3 Vec;
	float Dis;
	if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), Vec, Dis))
	{
		float R = Sph0.GetRadius();
		float Pen = R - Dis;
		glm::vec3 Norm = glm::normalize(Vec);
		Cont->Penetration = glm::abs(Pen);
		Cont->Normal = Norm;
		Cont->ContactPoint.push_back(Sph0.GetPos() - R * Norm);
		Temp.push_back(Cont);
	}
	else
	{
		glm::vec3 Norm;
		float R = Sph0.GetRadius();
		float Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(), Norm) + R;
		Cont->Penetration = Pen;
		Cont->Normal = -Norm;
		Cont->ContactPoint.push_back(Sph0.GetPos() - Pen * Norm);
		Temp.push_back(Cont);
	}
	return Temp;
}

std::vector<std::shared_ptr<Contacts>> ContactCreation::ContactCreate(Capsule Cap, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (std::shared_ptr<Sphere> Sphere0 = std::dynamic_pointer_cast<Sphere>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Cap, *Sphere0);
	}
	else if (std::shared_ptr<Capsule> Cap0 = std::dynamic_pointer_cast<Capsule>(Bod1->GetShapes()))
	{
		return this->S_Res->GetContacts(Cap, *Cap0);
	}
	std::vector<std::shared_ptr<Contacts>> Temp;
	glm::vec3 vec;
	float Pen;
	if (!this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), vec, Pen))
	{
		float R = Cap.GetRadius();
		Pen = glm::distance(glm::vec3(0.f), vec);
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
		std::shared_ptr<Contacts> Cont = std::make_shared<Contacts>();
		Cont->Normal = Norm;
		Cont->Penetration = R - Pen;
		for (auto& jj : Cap_Seg)
		{
			for (auto& ii : Obj_seg)
			{
				glm::vec3 RelNorm = MATH::Normalize(jj - ii);
				if (RelNorm == Norm)
				{
					Cont->ContactPoint = (jj + ii) / 2.f;
					break;
				}
			}
		}
		Temp.push_back(Cont);
	}
	else
	{
		std::shared_ptr<Contacts> Cont = std::make_shared<Contacts>();
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

std::vector<std::shared_ptr<Contacts>> ContactCreation::ContactCreate(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
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
	this->GJK_->EPA_GJK(Bod0->GetShapes(), Bod1->GetShapes(), Norm, Pen);
	if (Norm == glm::vec3(0.f) || isnan(glm::length(Norm)))
		Pen = this->SAT_->GetPenetrationContacts(Bod0->GetShapes(), Bod1->GetShapes(), Norm);
	//uses SAT to get contact points in one shot
	return this->SAT_->SAT_CreateContacts(Bod0->GetShapes(), Bod1->GetShapes(),
		Norm, Pen);
}

std::vector<std::shared_ptr<Contacts>> ContactCreation::MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
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

float ContactCreation::GetLowestFric(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (!Bod1->GetParticle())
		return Bod0->GetParticle()->GetFriction();
	float Fric[2] = { Bod0->GetParticle()->GetFriction(),Bod1->GetParticle()->GetFriction() };
	return (Fric[0] < Fric[1] ? Fric[0] : Fric[1]);
}

float ContactCreation::GetLowestRest(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	if (!Bod1->GetParticle())
		return Bod0->GetParticle()->GetRestitution();
	float Rest[2] = { Bod0->GetParticle()->GetRestitution(),Bod1->GetParticle()->GetRestitution() };
	return (Rest[0] < Rest[1] ? Rest[0] : Rest[1]);
}

ContactCreation::ContactCreation()
{
	this->S_Res = std::make_unique<ShapeResolution>();
	this->GJK_ = std::make_unique<GJK_Alg>();
	this->SAT_ = std::make_unique<SAT>();
}

ContactCreation::~ContactCreation()
{
}

std::vector<std::shared_ptr<Contacts>>ContactCreation::MakeManifold(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1, int ID)
{
	float Fric = GetLowestFric(Bod0, Bod1);
	float Rest = GetLowestRest(Bod0, Bod1);
	std::vector<std::shared_ptr<Contacts>> T = MakeContacts(Bod0, Bod1);
	for (auto& ii : T)
	{
		ii->Bods[0] = Bod0;
		ii->Bods[1] = Bod1;
		ii->Friction = Fric;
		ii->Restituion = Rest;
	}
	return T;
}

void ContactCreation::MakeJointManifold(std::vector<std::shared_ptr<Contacts>>& R, std::vector<std::shared_ptr<ColJoints>> BodJoints)
{
	for (auto& ii : BodJoints)
		ii->GetBodId();
}

