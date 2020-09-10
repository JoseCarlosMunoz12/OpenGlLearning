#include "ShapeResolution.h"
using namespace CoatlPhysicsEngine;

CoatlPhysicsEngine::ShapeResolution::ShapeResolution()
	:SphereRelSphere(),CapsuleRelSphere()
{
}

CoatlPhysicsEngine::ShapeResolution::~ShapeResolution()
{
}

template<typename _T, typename _N>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(_T T, _N N)
{
	return std::vector<std::shared_ptr<Contacts>>();
}

template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(Sphere T, Sphere N)
{
	return this->SphRelSph(T,N);
}
template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(Capsule N, Sphere T)
{
	return this->CapRelSph(N, T);
}

template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(Sphere T,Capsule N)
{
	return this->CapRelSph(N, T);
}

template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(Capsule N, Capsule T)
{
	return this->CapRel(N, T);
}