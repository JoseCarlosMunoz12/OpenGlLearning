#include "SphereRelAABB.h"
using namespace CoatlPhysicsEngine;
std::vector<std::shared_ptr<Contact>> SphereRelAABB::SphRelAABB(Sphere Sph0, AABB_Obj Obj)
{
	std::vector<std::shared_ptr<Contact>> Temp;
	std::shared_ptr<Contact> Cont = std::make_shared<Contact>();
	glm::vec3 Delta = Sph0.GetPos() - Obj.GetPos();
	glm::vec3 ClsPoint = Obj.GetClosesPoint(Sph0.GetPos());
	float Rad = Sph0.GetRadius();
	float Dis = glm::distance(ClsPoint,Sph0.GetPos());
	float Dot = Rad - Dis;
	glm::vec3 Norm = glm::normalize(Delta - ClsPoint);
	Cont->ContactPoint = Sph0.GetPos() - Norm * Rad;
	Cont->Penetration = Dot;
	Cont->Normal = Norm;
	Temp.push_back(Cont);
	return Temp;
}

SphereRelAABB::SphereRelAABB()
{
}

SphereRelAABB::~SphereRelAABB()
{
}
