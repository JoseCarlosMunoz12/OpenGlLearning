#include "CapsuleRelSphere.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contact>> CapsuleRelSphere::CapRelSph(Capsule Cap, Sphere Sph0)
{
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cont = std::make_shared<Contact>();
	float DisSqr = Cap.Distance(Sph0.GetPos());
	glm::vec3 Pos = Cap.ClosestPoint_P(Sph0.GetPos());
	float Total_R = Cap.GetRadius() + Sph0.GetRadius();
	glm::vec3 Norm = Pos - Sph0.GetPos();
	Norm = glm::normalize(Norm);
	glm::vec3 ContactPoint = Sph0.GetPos() - Norm * Sph0.GetRadius();
	Cont->Penetration = Total_R - DisSqr;
	Cont->Normal = -Norm;
	Cont->ContactPoint = ContactPoint;
	Temp.push_back(Cont);
	return Temp;
}

CapsuleRelSphere::CapsuleRelSphere()
{
}

CapsuleRelSphere::~CapsuleRelSphere()
{
}
