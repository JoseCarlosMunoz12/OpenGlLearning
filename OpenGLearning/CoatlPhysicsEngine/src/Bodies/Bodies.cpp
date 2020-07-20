#include "Bodies.h"
using namespace CoatlPhysicsEngine;

Bodies::Bodies(int InitID)
	:Max(glm::vec3(0.f)), Min(glm::vec3(0.f))
{
	this->ID = InitID;
	this->Vec_Size = BodyInf.size();
}

Bodies::Bodies( std::shared_ptr<ColShapes> InitShapes, int InitID)
	:Max(glm::vec3(0.f)), Min(glm::vec3(0.f))
{
	this->ID = InitID;
	int Count = this->BodyInf.size();
	this->BodyInf.push_back(std::make_shared<BodyParts>(InitShapes));
	this->Vec_Size = BodyInf.size();
}

Bodies::Bodies(std::vector< std::shared_ptr<ColShapes>> InitShapes, int InitID)
	:Max(glm::vec3(0.f)), Min(glm::vec3(0.f))
{
	this->ID = InitID;
	int Count = this->BodyInf.size();
	for (auto& ii : InitShapes)
	{
		this->BodyInf.push_back(std::make_shared<BodyParts>(ii));
	}
	this->Vec_Size = BodyInf.size();
}

Bodies::~Bodies()
{
}

void Bodies::AddShapes(std::shared_ptr<ColShapes> NewShape, int Parent)
{
	if (Parent  < 0)
		this->BodyInf.push_back(std::make_shared<BodyParts>(NewShape));
	else
		this->BodyInf.push_back(std::make_shared<BodyParts>(this->BodyInf[Parent],NewShape));
	this->Vec_Size = BodyInf.size();
}


int Bodies::GetID()
{
	return this->ID;
}

int CoatlPhysicsEngine::Bodies::GetVecSize()
{
	return this->Vec_Size;
}

void Bodies::SetPosition(glm::vec3 NewPos)
{
	this->BodyInf[0]->SetPos(NewPos);
}

void Bodies::UpdateAABB()
{
	Max = this->BodyInf[0]->Support(Units[0]);
	Min = this->BodyInf[0]->Support(Units[3]);
	for (auto& ii : this->BodyInf)
	{
		for (auto& jj : Units)
		{
			glm::vec3 T = ii->Support(jj);
			Max = MATH::SetMax(Max, T);
			Min = MATH::SetMin(Min, T);
		}
	}
	glm::vec3 Mid = (Max + Min) / 2.f;
}


void Bodies::MovePosition(glm::vec3 Add)
{
	glm::vec3 OldPos = this->BodyInf[0]->GetPos();
	OldPos += Add;
	this->SetPosition(OldPos);
}

void Bodies::SetParticle(int ShapeID)
{
	this->BodyInf[ShapeID]->AddParticle(std::make_shared<Particle>(this->GetPos()));
}

void Bodies::SetQuat(glm::quat NewQuat)
{
	this->BodyInf[0]->SetQuat(NewQuat);
}

glm::vec3 Bodies::GetPos()
{
	return this->BodyInf[0]->GetPos();	
}

glm::quat Bodies::GetQuat()
{
	return this->BodyInf[0]->GetQuatAngle();
}

std::shared_ptr<ColShapes> Bodies::GetShapes()
{
	return this->BodyInf[0]->GetShape();
}

std::vector<std::shared_ptr<BodyParts>> Bodies::GetBodyParts()
{
	return this->BodyInf;
}

std::shared_ptr<BodyParts> Bodies::GetSpecificBodyPart(int ID)
{
	return this->BodyInf[ID];
}

std::shared_ptr<Particle> CoatlPhysicsEngine::Bodies::GetParticle(int ID)
{
	return this->BodyInf[ID]->GetParticle();
}
