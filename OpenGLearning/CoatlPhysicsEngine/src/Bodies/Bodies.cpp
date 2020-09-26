#include "Bodies.h"
using namespace CoatlPhysicsEngine;

Bodies::Bodies(int InitID)
	:Max(glm::vec3(0.f)), Min(glm::vec3(0.f)), Mid(glm::vec3(0.f))
{
	this->ID = InitID;
}

Bodies::Bodies(std::shared_ptr<ColShapes> InitShapes, int InitID)
	:Max(glm::vec3(0.f)), Min(glm::vec3(0.f)), Mid(glm::vec3(0.f))
{
	this->ID = InitID;
	this->BodyInf = std::make_shared<BodyParts>(InitShapes);
}

Bodies::~Bodies()
{
}

void Bodies::AddShapes(std::shared_ptr<ColShapes> NewShape)
{
	this->BodyInf = std::make_shared<BodyParts>(NewShape);
}

int Bodies::GetID()
{
	return this->ID;
}

void Bodies::SetPosition(glm::vec3 NewPos)
{
	this->BodyInf->SetPos(NewPos);
}

void Bodies::UpdateAABB()
{
	Max = this->BodyInf->Support(Units[0]);
	Min = this->BodyInf->Support(Units[3]);
	for (auto& jj : Units)
	{
		glm::vec3 T = this->BodyInf->Support(jj);
		Max = MATH::SetMax(Max, T);
		Min = MATH::SetMin(Min, T);
	}
	this->Mid = (Max + Min) / 2.f;
}

void Bodies::MovePosition(glm::vec3 Add)
{
	glm::vec3 OldPos = this->BodyInf->GetPos();
	OldPos += Add;
	this->SetPosition(OldPos);
}

void Bodies::SetParticle(int ShapeID)
{
	this->BodyInf->AddParticle(std::make_shared<Particle>(this->GetPos()));
}

void Bodies::SetRigidBody(int ShapeID)
{
	this->BodyInf->AddParticle(std::make_shared<RigidBodies>(this->GetPos()));
}

void Bodies::SetQuat(glm::quat NewQuat)
{
	this->BodyInf->SetQuat(NewQuat);
}

bool Bodies::HasId(std::shared_ptr<Bodies> OtherBod)
{
	return std::find(BoolId.begin(), BoolId.end(), OtherBod->GetID()) != BoolId.end();
}

bool Bodies::HasCollided()
{
	return this->BoolId.size();
}

void Bodies::AddId(int NewId)
{
	this->BoolId.push_back(NewId);
}

void Bodies::RemoveID(int RevId)
{
	std::vector<int>::iterator it = std::find(this->BoolId.begin(), this->BoolId.end(), RevId);
	if (it != this->BoolId.end())
		this->BoolId.erase(it);
}

glm::vec3 Bodies::GetPos()
{
	return this->BodyInf->GetPos();	
}

glm::quat Bodies::GetQuat()
{
	return this->BodyInf->GetQuatAngle();
}

std::shared_ptr<ColShapes> Bodies::GetShapes()
{
	return this->BodyInf->GetShape();
}

std::shared_ptr<BodyParts> Bodies::GetBodyParts()
{
	return this->BodyInf;
}


std::shared_ptr<Bod_Base> Bodies::GetParticle()
{
	return this->BodyInf->GetParticle();
}
