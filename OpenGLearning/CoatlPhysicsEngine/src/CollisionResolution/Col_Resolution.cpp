#include "Col_Resolution.h"
using namespace CoatlPhysicsEngine;
std::vector<std::shared_ptr<Contact>>Col_Resolution::MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	Sphere S(glm::vec3(0.f),2);
	Sphere S1(glm::vec3(1.f), 2);
	return this->S_Res->GetContacts(S,S);
}
Col_Resolution::Col_Resolution()
{
	this->S_Res = std::make_unique<ShapeResolution>();
}

Col_Resolution::~Col_Resolution()
{
}

std::shared_ptr<Manifold> Col_Resolution::MakeManifold(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	std::shared_ptr<Manifold> Temp = std::make_shared<Manifold>();
	Temp->Bod0 = Bod0;
	Temp->Bod1 = Bod1;
	Temp->Contacts = MakeContacts(Bod0, Bod1);
	Temp->ContactCount = Temp->Contacts.size();
	return Temp;
}
