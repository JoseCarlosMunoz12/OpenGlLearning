#include "SphereRelSphere.h"
using namespace CoatlPhysicsEngine;
std::vector<std::shared_ptr<Contact>> SphereRelSphere::SphRelSph(Sphere Sph0, Sphere Sph1)
{
	std::vector<std::shared_ptr<Contact>> Contacts;
	std::shared_ptr<Contact> Temp = std::make_shared<Contact>();
	float Distance = glm::distance(Sph0.GetPos(), Sph1.GetPos());
	float TotalR = Sph0.GetRadius() + Sph1.GetRadius();
	glm::vec3 Norm = Sph1.GetPos() - Sph0.GetPos();
	Norm = glm::normalize(Norm);
	glm::vec3 ContactPoint = Sph0.GetPos() - Norm * Sph0.GetRadius();
	Temp->Penetration = TotalR - Distance;
	Temp->Normal = -Norm;
	Temp->ContactPoint = ContactPoint;
	Contacts.push_back(Temp);
	return Contacts;
}

SphereRelSphere::SphereRelSphere()
{
}

SphereRelSphere::~SphereRelSphere()
{
}
