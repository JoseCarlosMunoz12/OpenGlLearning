#include "Col_Resolution.h"
using namespace CoatlPhysicsEngine;
std::vector<std::shared_ptr<Contact>>Col_Resolution::MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1)
{
	return std::vector<std::shared_ptr<Contact>>();
}
Col_Resolution::Col_Resolution()
{
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
	Temp->ContactCountl = Temp->Contacts.size();
	return Temp;
}

template<typename _T, typename _N>
inline std::vector<std::shared_ptr<Contact>> Col_Resolution::Create(_T R, _N N)
{
	return std::vector<std::shared_ptr<Contact>>();
}

template<>
std::vector<std::shared_ptr<Contact>> Col_Resolution::Create(Sphere R, Sphere N)
{
	return std::vector<std::shared_ptr<Contact>>();
}
