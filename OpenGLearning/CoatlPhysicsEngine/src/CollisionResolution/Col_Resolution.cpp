#include "Col_Resolution.h"
using namespace CoatlPhysicsEngine;

void Col_Resolution::PrepeareContact(std::shared_ptr<Manifold> Cnt, float dt)
{
	for (auto& jj : Cnt->Contacts)
		jj->CalculateInternals(Cnt->Bods, dt);
}

void Col_Resolution::AdjustPosition(std::shared_ptr<Manifold> Cnt, float dt)
{
	int Index;
	glm::vec3 LinChng[2],AngCh[2];
	float Max;
	glm::vec3 DelPos;
	int ItUsed = 0;
	while (ItUsed < Posit)
	{
		int NumCount = Cnt->ContactCount;
		Index = Cnt->ContactCount;
		Max = 0.0001f;
		for (int ii = 0; ii < NumCount; ii++)
		{
			if (Cnt->Contacts[ii]->Penetration > Max)
			{
				Max = Cnt->Contacts[ii]->Penetration;
				Index = ii;
			}
		}
		if (Index == NumCount) break;
		Cnt->Contacts[Index]->MatchAwakeState(Cnt->Bods);
		//Reseolve Penetration
		Cnt->Contacts[Index]->ApplyPositionChange(Cnt->Bods, LinChng, AngCh);
		//May change the penetration, so update those contacts
		for (int ii = 0; ii < NumCount; ii++)
		{
			for (int bb = 0; bb < 2; bb++) if (Cnt->Bods[bb]->GetParticle())
			{
				for (int dd = 0; dd < 2; dd++)
				{
					if (Cnt->Bods[bb]->GetID() == Cnt->Bods[dd]->GetID())
					{
						DelPos = LinChng[dd] +
							glm::cross(AngCh[dd], Cnt->Contacts[ii]->RelContact[bb]);
						Cnt->Contacts[ii]->Penetration += glm::dot(DelPos, Cnt->Contacts[ii]->Normal) * (bb ? 1 : -1);
					}
				}
			}
		}
		ItUsed++;
	}
}

void Col_Resolution::AdjustVelocity(std::shared_ptr<Manifold> Cnt, float dt)
{
	int Index;
	glm::vec3 VelChng[2], RotCh[2];
	
	glm::vec3 DelPos;
	int ItUsed = 0;
	int NumContacts = Cnt->ContactCount;
	while (ItUsed < VelIt)
	{
		int Index = NumContacts;
		float Max = 0.0001;
		for (int ii = 0; ii < NumContacts; ii++)
		{
			if (Cnt->Contacts[ii]->DesDeltaVel > Max)
			{
				Max = Cnt->Contacts[ii]->DesDeltaVel;
				Index = ii;
			}
		}
		if (Index == NumContacts)break;
		Cnt->Contacts[Index]->MatchAwakeState(Cnt->Bods);
		Cnt->Contacts[Index]->ApplyVelocityChange(Cnt->Bods, VelChng, RotCh);
		for (int ii = 0; ii < NumContacts; ii++)
		{
			for (int bb = 0; bb < 2; bb++)if (Cnt->Bods[bb]->GetParticle())
			{
				for (int dd = 0; dd < 2; dd++)
				{
					if (Cnt->Bods[bb]->GetID() == Cnt->Bods[dd]->GetID())
					{
						DelPos = VelChng[dd] +
							glm::cross(RotCh[dd], Cnt->Contacts[ii]->RelContact[bb]);
						glm::vec3 V = glm::transpose(Cnt->Contacts[ii]->ContactToWorld) * DelPos ;
						V = (bb ? -1.f : 1.f) * V;
						Cnt->Contacts[ii]->ContactVelocity += V;
						Cnt->Contacts[ii]->CalculateDesVel(Cnt->Bods, dt);
					}
				}
			}
		}
		ItUsed++;
	}
}

void Col_Resolution::ResolveResolution(std::shared_ptr<Bodies> Bod, std::shared_ptr<Manifold> Cnt)
{
	glm::vec3 Norm = Cnt->Contacts[0]->Normal;
	float Diff = Cnt->Contacts[0]->Penetration;
	if (Diff < 0.0001)
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

Col_Resolution::Col_Resolution(int VelInit,int PosInit)
	:VelIt(VelInit), Posit(PosInit)
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
	Temp->Bods[0] = Bod0;
	Temp->Bods[1] = Bod1;
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
	/*if (Cnt->ContactCount > 0)
	{
		float Diff = Cnt->Contacts[0]->Penetration;
		if (Diff < 0.0001)
			Diff = 0.f;
		glm::vec3 Norm = Cnt->Contacts[0]->Normal;
		switch (Cnt->ID)
		{
		case 0:
			this->ResolveResolution(Cnt->Bods[0], Cnt);
			break;
		case 1:
			this->ResolveResolution(Cnt->Bods[1], Cnt);
			break;
		case 2:
			break;
		default:
			if (Cnt->Bods[0]->GetBodyParts()->GetParticle())
			{
				Cnt->Bods[0]->MovePosition(Diff * Norm / 2.f);
			}
			if (Cnt->Bods[1]->GetBodyParts()->GetParticle())
			{
				Cnt->Bods[1]->MovePosition(-Diff * Norm / 2.f);
			}
			break;
		}
	}*/

	//Prepare bodies for resolution
	this->PrepeareContact(Cnt, dt);
	//Adjust the positions
	this->AdjustPosition(Cnt, dt);
	//adjust the velocities of the bodies
	this->AdjustVelocity(Cnt, dt);
}
