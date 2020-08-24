#include "ShapeResolution.h"
using namespace CoatlPhysicsEngine;

CoatlPhysicsEngine::ShapeResolution::ShapeResolution()
	:SphereRelSphere(),SphereRelAABB(),AABBRelAABB(),CapsuleRelSphere(),CapsuleRelAABB()
{
}

CoatlPhysicsEngine::ShapeResolution::~ShapeResolution()
{
}

template<typename _T, typename _N>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(_T T, _N N)
{
	return std::vector<std::shared_ptr<Contact>>();
}

template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(Sphere T, Sphere N)
{
	return this->SphRelSph(T,N);
}

template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(AABB_Obj T, AABB_Obj N)
{
	return this->AABB_Rel(T, N);
}

template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(Sphere T, AABB_Obj N)
{
	return this->SphRelAABB(T, N);
}

template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(AABB_Obj N, Sphere T )
{
	return this->SphRelAABB(T, N);
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
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(AABB_Obj T, Capsule N)
{
	return this->CapRelAABB(N, T);
}

template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(Capsule N,AABB_Obj T)
{
	return this->CapRelAABB(N, T);
}
template<>
std::vector<std::shared_ptr<Contacts>> ShapeResolution::GetContacts(Capsule N, Capsule T)
{
	return this->CapRel(N, T);
}