#include "SphereRelSphere.h"
using namespace CoatlPhysicsEngine;
std::vector<std::shared_ptr<Contacts>> SphereRelSphere::SphRelSph(Sphere Sph0, Sphere Sph1)
{
	std::vector<std::shared_ptr<Contacts>> Contact;
	std::shared_ptr<Contacts> Temp = std::make_shared<Contacts>();	
	float TotalR = Sph0.GetRadius() + Sph1.GetRadius();
	glm::vec3 MidLine = Sph1.GetPos() - Sph0.GetPos();
	float Size = glm::length(MidLine);
	glm::vec3 Norm = glm::vec3(1.f, 0.f, 0.f);
	if (Size == 0.f)
		Size = TotalR;
	else
		Norm = -MidLine / Size;
	Temp->Normal = Norm;
	Temp->Penetration = TotalR - Size;
	Temp->ContactPoint.push_back(Sph1.GetPos() - MidLine * .5f);
	Temp->R0.push_back(-Norm * Sph0.GetRadius());
	Temp->R1.push_back(Norm * Sph1.GetRadius());
	Contact.push_back(Temp);
	return Contact;
}

SphereRelSphere::SphereRelSphere()
{
}

SphereRelSphere::~SphereRelSphere()
{
}
