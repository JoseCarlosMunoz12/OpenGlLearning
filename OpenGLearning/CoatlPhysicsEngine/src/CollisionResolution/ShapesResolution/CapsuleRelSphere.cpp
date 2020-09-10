#include "CapsuleRelSphere.h"
using namespace CoatlPhysicsEngine;

std::vector<std::shared_ptr<Contacts>> CapsuleRelSphere::CapRelSph(Capsule Cap, Sphere Sph0)
{
	std::vector<std::shared_ptr<Contacts>> Temp;
	std::shared_ptr<Contacts> Cont = std::make_shared<Contacts>();
	float DisSqr = Cap.Distance(Sph0.GetPos());
	glm::vec3 Pos = MATH::ClosestPoint_Seg(Cap.GetSegment(), Sph0.GetPos());
	float Total_R = Cap.GetRadius() + Sph0.GetRadius();
	glm::vec3 Norm = Pos - Sph0.GetPos();
	Norm = MATH::Normalize(Norm);
	glm::vec3 ContactPoint = Sph0.GetPos() + Norm * Sph0.GetRadius();
	Cont->Penetration = Total_R - DisSqr;
	Cont->Normal = -Norm;
	Cont->ContactPoint.push_back(ContactPoint);
	Temp.push_back(Cont);
	return Temp;
}

CapsuleRelSphere::CapsuleRelSphere()
{
}

CapsuleRelSphere::~CapsuleRelSphere()
{
}
