#include "Col_Resolution.h"
using namespace CoatlPhysicsEngine;

void Col_Resolution::PrepeareContact(std::vector<std::shared_ptr<Contacts>> Cnt, float dt)
{
	for (auto& jj : Cnt)
		jj->CalculateInternals(dt);
}

void Col_Resolution::AdjustPosition(std::vector<std::shared_ptr<Contacts>> Cnt, float dt)
{
	int Index;
	glm::vec3 LinChng[2],AngCh[2];
	float Max;
	glm::vec3 DelPos;
	int ItUsed = 0;
	while (ItUsed < Posit)
	{
		int NumCount = Cnt.size();
		Index = NumCount;
		Max = 0.01f;
		for (int ii = 0; ii < NumCount; ii++)
		{
			if (Cnt[ii]->Penetration > Max)
			{
				Max = Cnt[ii]->Penetration;
				Index = ii;
			}
		}
		if (Index == NumCount) break;
		Cnt[Index]->MatchAwakeState();
		//Reseolve Penetration
		Cnt[Index]->ApplyPositionChange(LinChng, AngCh);
		//May change the penetration, so update those contacts
		for (int ii = 0; ii < NumCount; ii++)
		{
			for (int bb = 0; bb < 2; bb++) if (Cnt[ii]->Bods[bb]->GetParticle())
			{
				for (int dd = 0; dd < 2; dd++)
				{
					if (Cnt[ii]->Bods[bb]->GetID() == Cnt[Index]->Bods[dd]->GetID())
					{
						DelPos = LinChng[dd] +
							glm::cross(AngCh[dd], Cnt[ii]->RelContact[bb]);
						Cnt[ii]->Penetration += glm::dot(DelPos, Cnt[ii]->Normal) * (bb ? 1 : -1);
					}
				}
			}
		}
		ItUsed++;
	}
}

void Col_Resolution::AdjustVelocity(std::vector<std::shared_ptr<Contacts>> Cnt, float dt)
{
	int Index;
	glm::vec3 VelChng[2], RotCh[2];
	
	glm::vec3 DelPos;
	int ItUsed = 0;
	int NumContacts = Cnt.size();
	while (ItUsed < VelIt)
	{
		int Index = NumContacts;
		float Max = 0.01;
		for (int ii = 0; ii < NumContacts; ii++)
		{
			if (Cnt[ii]->DesDeltaVel > Max)
			{
				Max = Cnt[ii]->DesDeltaVel;
				Index = ii;
			}
		}
		if (Index == NumContacts)break;
		Cnt[Index]->MatchAwakeState();
		Cnt[Index]->ApplyVelocityChange(VelChng, RotCh);
		for (int ii = 0; ii < NumContacts; ii++)
		{
			for (int bb = 0; bb < 2; bb++)if (Cnt[ii]->Bods[bb]->GetParticle())
			{
				for (int dd = 0; dd < 2; dd++)
				{
					if (Cnt[ii]->Bods[bb]->GetID() == Cnt[Index]->Bods[dd]->GetID())
					{
						DelPos = VelChng[dd] +
							glm::cross(RotCh[dd], Cnt[ii]->RelContact[bb]);
						glm::vec3 V = glm::transpose(Cnt[ii]->ContactToWorld) * DelPos ;
						V = (bb ? -1.f : 1.f) * V;
						Cnt[ii]->ContactVelocity += V;
						Cnt[ii]->CalculateDesVel(dt);
					}
				}
			}
		}
		ItUsed++;
	}
}

std::vector<std::shared_ptr<Contacts>> Col_Resolution::ContactCreate(Sphere Sph0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
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

std::vector<std::shared_ptr<Contacts>> Col_Resolution::ContactCreate(Capsule Cap, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
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
					std::shared_ptr<Contacts> Cont = std::make_shared<Contacts>();
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

std::vector<std::shared_ptr<Contacts>> Col_Resolution::ContactCreate(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
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

std::vector<std::shared_ptr<Contacts>> Col_Resolution::MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
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
	std::vector<std::shared_ptr<Contacts>> T = MakeContacts(Bod0, Bod1);
	for (auto& ii : T)
	{
		ii->Bods[0] = Bod0;
		ii->Bods[1] = Bod1;
	}


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
	//Prepare bodies for resolution
	this->PrepeareContact(Cnt, dt);
	//Adjust the positions
	this->AdjustPosition(Cnt, dt);
	//adjust the velocities of the bodies
	this->AdjustVelocity(Cnt, dt);
}

/////////////////////////////////////////////////////////////////////////
//----------------------------Contact class----------------------------//
/////////////////////////////////////////////////////////////////////////

glm::mat3 Contacts::MakeSkew(glm::vec3 Vec)
{
	glm::vec3 Col0(0.f, Vec.z, -Vec.y);
	glm::vec3 Col1(-Vec.z, 0.f, Vec.x);
	glm::vec3 Col2(Vec.y, -Vec.x, 0.f);
	return glm::mat3(Col0, Col1, Col2);
}

void Contacts::CalculateContactbasis()
{
	glm::vec3 ContactTanget[2];
	if (glm::abs(Normal.x) > glm::abs(Normal.y))
	{
		float S = 1.f / glm::sqrt(Normal.z * Normal.z + Normal.x * Normal.x);
		//new X-Axis is at right angles ot eh world X-Axis
		ContactTanget[0].x = Normal.z * S;
		ContactTanget[0].y = 0.f;
		ContactTanget[0].z = -Normal.x * S;
		//the New Y Axis ist at right angles to the new x- Z- axis
		ContactTanget[1].x = Normal.y * ContactTanget[0].x;
		ContactTanget[1].y = Normal.z * ContactTanget[0].x - Normal.x * ContactTanget[0].z;
		ContactTanget[1].z = -Normal.y * ContactTanget[0].x;
	}
	else
	{
		float S = 1.f / glm::sqrt(Normal.z * Normal.z + Normal.y * Normal.y);
		//new X-Axis is at right angles ot eh world X-Axis
		ContactTanget[0].x = 0.f;
		ContactTanget[0].y = -Normal.z * S;
		ContactTanget[0].z = Normal.y * S;
		//the New Y Axis ist at right angles to the new x- Z- axis
		ContactTanget[1].x = Normal.y * ContactTanget[0].z - Normal.z * ContactTanget[0].y;
		ContactTanget[1].y = -Normal.x * ContactTanget[0].z;
		ContactTanget[1].z = Normal.x * ContactTanget[0].y;

	}
	this->ContactToWorld = glm::mat3(Normal, ContactTanget[0], ContactTanget[1]);
}

glm::vec3 Contacts::CalculateLocalvel(int ID, float dt)
{
	glm::vec3 ContactVel = glm::vec3(0.f);
	if (!Bods[ID]->GetParticle())
		return ContactVel;
	glm::vec3 Vel = glm::cross(Bods[ID]->GetParticle()->GetRotVel(), RelContact[ID]);
	Vel += Bods[ID]->GetParticle()->GetVel();
	ContactVel = glm::transpose(this->ContactToWorld) * Vel;
	glm::vec3 Acc = Bods[ID]->GetParticle()->GetAccel();
	glm::vec3 AccVel = Acc * dt;
	AccVel = glm::transpose(this->ContactToWorld) * AccVel;
	AccVel.x = 0.f;
	ContactVel += AccVel;
	return ContactVel;
}

glm::vec3 Contacts::CalcFricImpulse(glm::mat3 InvInTn[2])
{
	glm::vec3 ImpContact = glm::vec3(0.f);
	float InvMass = 1.f / Bods[0]->GetParticle()->GetMass();
	glm::mat3 ImpToTorque = this->MakeSkew(RelContact[0]);

	glm::mat3 DelVelWorld0 = ImpToTorque;
	DelVelWorld0 *= InvInTn[0];
	DelVelWorld0 *= ImpToTorque;
	DelVelWorld0 *= -1;

	if (Bods[1]->GetParticle())
	{
		ImpToTorque = this->MakeSkew(RelContact[1]);
		glm::mat3 DelVelWorld1 = ImpToTorque;
		DelVelWorld1 *= InvInTn[1];
		DelVelWorld1 *= ImpToTorque;
		DelVelWorld1 *= -1;
		DelVelWorld0 += DelVelWorld1;
		InvMass += (1 / Bods[1]->GetParticle()->GetMass());
	}

	glm::mat3 DelVel = glm::transpose(ContactToWorld);
	DelVel *= DelVelWorld0;
	DelVel *= ContactToWorld;

	DelVel[0].x += InvMass;
	DelVel[1].y += InvMass;
	DelVel[2].z += InvMass;

	glm::mat3 ImpMatrix = glm::inverse(DelVel);

	glm::vec3 VelKill(DesDeltaVel, -ContactVelocity.y, -ContactVelocity.z);
	ImpContact = ImpMatrix * VelKill;

	float PlanarImpulse = glm::sqrt(ImpContact.y * ImpContact.y + ImpContact.z * ImpContact.z);
	if (PlanarImpulse > (ImpContact.x * Friction))
	{
		ImpContact.y /= PlanarImpulse;
		ImpContact.z /= PlanarImpulse;

		ImpContact.x = DelVel[0].x +
			DelVel[1].x * Friction * ImpContact.y +
			DelVel[2].x * Friction * ImpContact.z;

		ImpContact.x = DesDeltaVel / ImpContact.x;
		ImpContact.y *= Friction * ImpContact.x;
		ImpContact.z *= Friction * ImpContact.x;
	}
	return ImpContact;
}

glm::vec3 Contacts::CalcNonFricImpulse(glm::mat3 InvInTn[2])
{
	glm::vec3 ImpContact = glm::vec3(0.f);
	glm::vec3 DelVelWorld = glm::cross(RelContact[0], Normal);
	DelVelWorld = InvInTn[0] * DelVelWorld;
	DelVelWorld = glm::cross(DelVelWorld, RelContact[0]);

	float DelVel = glm::dot(DelVelWorld, Normal);
	DelVel += (1.f / Bods[0]->GetParticle()->GetMass());

	if (Bods[1]->GetParticle())
	{
		glm::vec3 DelVelWorld0 = glm::cross(RelContact[1], Normal);
		DelVelWorld0 = InvInTn[1] * DelVelWorld0;
		DelVelWorld0 = glm::cross(DelVelWorld0, RelContact[1]);
		DelVel += glm::dot(DelVelWorld0, Normal);
		DelVel += (1.f / Bods[1]->GetParticle()->GetMass());
	}

	ImpContact.x = DesDeltaVel / DelVel;
	ImpContact.y = 0.f;
	ImpContact.z = 0.f;

	return ImpContact;
}

void Contacts::CalculateDesVel(float dt)
{
	const static float VelLimit = 0.25f;
	float VelFromAcc = 0.f;

	if (Bods[0]->GetParticle()->GetAwakeStatus())
		VelFromAcc += dt * (glm::dot(Bods[0]->GetParticle()->GetAccel(), Normal));
	if (Bods[1]->GetParticle() && Bods[1]->GetParticle()->GetAwakeStatus())
		VelFromAcc -= dt * (glm::dot(Bods[1]->GetParticle()->GetAccel(), Normal));
	float ThisRest = Restituion;
	if (glm::abs(ContactVelocity.x) < VelLimit)
		ThisRest = 0.f;
	this->DesDeltaVel = -ContactVelocity.x - ThisRest * (ContactVelocity.x - VelFromAcc);
}

void Contacts::MatchAwakeState()
{
	if (!Bods[1]->GetParticle())
		return;
	bool Bod0Awake = Bods[0]->GetParticle()->GetAwakeStatus();
	bool Bod1Awake = Bods[1]->GetParticle()->GetAwakeStatus();
	if (Bod0Awake ^ Bod1Awake)
		if (Bod0Awake)
			Bods[1]->GetParticle()->SetAwake(true);
		else
			Bods[0]->GetParticle()->SetAwake(true);
}

void Contacts::CalculateInternals(float dt)
{
	this->CalculateContactbasis();
	this->RelContact[0] = ContactPoint - Bods[0]->GetPos();
	this->RelContact[1] = ContactPoint - Bods[1]->GetPos();
	this->ContactVelocity = this->CalculateLocalvel(0, dt);
	this->ContactVelocity -= this->CalculateLocalvel(1, dt);
	this->CalculateDesVel(dt);
}

void Contacts::ApplyPositionChange(glm::vec3 LinChang[2], glm::vec3 AngChange[2])
{
	const float AngLimit = .2f;
	float AngM[2];
	float LinM[2];

	float TotalInertia = 0.f;
	float LinInertia[2];
	float Anginertia[2];
	//Calculate inertia of each object in the direction of the contact normal
	for (int ii = 0; ii < 2; ii++) if (Bods[ii]->GetParticle())
	{
		glm::mat3 invInert = Bods[ii]->GetParticle()->GetInertiaWorld();
		invInert = glm::inverse(invInert);
		glm::vec3 AngInertW = glm::cross(RelContact[ii], Normal);
		AngInertW = invInert * AngInertW;
		AngInertW = glm::cross(AngInertW, RelContact[ii]);
		Anginertia[ii] = glm::dot(AngInertW, Normal);
		LinInertia[ii] = 1 / Bods[ii]->GetParticle()->GetMass();
		TotalInertia += LinInertia[ii] + Anginertia[ii];
	}
	//loop through again calculating and applying the changes
	for (int ii = 0; ii < 2; ii++)if (Bods[ii]->GetParticle())
	{
		float Sign = (ii == 0) ? 1 : -1;
		AngM[ii] = Sign * Penetration * (Anginertia[ii] / TotalInertia);
		LinM[ii] = Sign * Penetration * (LinInertia[ii] / TotalInertia);

		glm::vec3 Proj = RelContact[ii] - Normal * glm::dot(RelContact[ii], Normal);
		float MaxMag = AngLimit * glm::length(Proj);

		if (AngM[ii] < -MaxMag)
		{
			float TtlMv = AngM[ii] + LinM[ii];
			AngM[ii] = -MaxMag;
			LinM[ii] = TtlMv - AngM[ii];
		}
		else if (AngM[ii] > MaxMag)
		{
			float TtlMv = AngM[ii] + LinM[ii];
			AngM[ii] = MaxMag;
			LinM[ii] = TtlMv - AngM[ii];
		}

		if (AngM[ii] == 0)
			AngChange[ii] = glm::vec3(0.f);
		else
		{
			glm::vec3 TargetAngDir = glm::cross(RelContact[ii], Normal);
			glm::mat3 invIn = Bods[ii]->GetParticle()->GetInertiaWorld();
			invIn = glm::inverse(invIn);
			AngChange[ii] = invIn * TargetAngDir * (AngM[ii] / Anginertia[ii]);
		}

		LinChang[ii] = Normal * LinM[ii];

		glm::vec3 Pos = Bods[ii]->GetPos();
		Pos += Normal * LinM[ii];
		Bods[ii]->SetPosition(Pos);

		glm::quat Q = Bods[ii]->GetQuat();
		glm::quat R = glm::quat(0.f, AngChange[ii]);
		R *= Q;
		Q += 0.5f * R;
		Bods[ii]->SetQuat(Q);
		if (!Bods[ii]->GetParticle()->GetAwakeStatus())
			Bods[ii]->GetParticle()->CalcDerivedData();
	}
}

void Contacts::ApplyVelocityChange(glm::vec3 VelChang[2], glm::vec3 RotChange[2])
{
	glm::mat3 InvInertia[2];
	//Get Inertia Tensor
	InvInertia[0] = glm::inverse(Bods[0]->GetParticle()->GetInertiaWorld());
	if (Bods[1]->GetParticle())
		InvInertia[1] = glm::inverse(Bods[1]->GetParticle()->GetInertiaWorld());
	//Calc Impulse
	glm::vec3 ImpulseCont;
	if (Friction != 0)
		ImpulseCont = this->CalcFricImpulse(InvInertia);
	else
		ImpulseCont = this->CalcNonFricImpulse(InvInertia);
	//Convert impulse to World Coordinates
	glm::vec3 Impulse = this->ContactToWorld * ImpulseCont;
	//Split impulse to linear and roation parts
	glm::vec3 ImTrque0 = glm::cross(RelContact[0], Impulse);
	RotChange[0] = InvInertia[0] * ImTrque0;
	VelChang[0] = glm::vec3(0.f);
	VelChang[0] = Impulse * 1.f / Bods[0]->GetParticle()->GetMass();

	//Apply Changes
	Bods[0]->GetParticle()->AddVel(VelChang[0]);
	Bods[0]->GetParticle()->AddRotVel(RotChange[0]);

	//Body 2 changes
	if (Bods[1]->GetParticle())
	{
		//Split impulse to linear and roation parts
		glm::vec3 ImTrque1 = glm::cross(RelContact[1], Impulse);
		RotChange[1] = InvInertia[1] * ImTrque1;
		VelChang[1] = glm::vec3(0.f);
		VelChang[1] = Impulse * -1.f / Bods[1]->GetParticle()->GetMass();
		//Apply Changes
		Bods[1]->GetParticle()->AddVel(VelChang[1]);
		Bods[1]->GetParticle()->AddRotVel(RotChange[1]);
	}
}
