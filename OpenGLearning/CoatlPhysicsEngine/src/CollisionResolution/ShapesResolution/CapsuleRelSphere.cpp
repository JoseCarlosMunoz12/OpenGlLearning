#include "CapsuleRelSphere.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contact>> CapsuleRelSphere::CapRelSph(Capsule Cap, Sphere Sph0)
{
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cont = std::make_shared<Contact>();
	float DisSqr = Cap.Distance(Sph0.GetPos());
	glm::vec3 Pos = MATH::ClosestPoint_Seg(Cap.GetSegment(), Sph0.GetPos());
	float Total_R = Cap.GetRadius() + Sph0.GetRadius();
	glm::vec3 Norm = Pos - Sph0.GetPos();
	Norm = -Norm / glm::length(Norm);
	glm::vec3 ContactPoint = Sph0.GetPos() + Norm * Sph0.GetRadius();
	Cont->Penetration = Total_R - DisSqr;
	Cont->Normal = Norm;
	Cont->ContactPoint.push_back(ContactPoint);
	//gets sphere contact location
	glm::vec3 R0 = -Norm * Sph0.GetRadius();
	Cont->R0.push_back(R0);
	//Gets Capsule Contact Location
 	glm::vec3 R1 = (Sph0.GetPos() + R0 + Cont->Penetration * Norm) - Cap.GetPos();
	Cont->R1.push_back(R1);
	Temp.push_back(Cont);
	return Temp;
}

CapsuleRelSphere::CapsuleRelSphere()
{
}

CapsuleRelSphere::~CapsuleRelSphere()
{
}
