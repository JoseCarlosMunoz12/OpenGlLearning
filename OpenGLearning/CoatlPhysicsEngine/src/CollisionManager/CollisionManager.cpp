#include "CollisionManager.h"
using namespace CoatlPhysicsEngine;

CollisionManager::CollisionManager()
	:SphereColSphere()
{

}

CollisionManager::~CollisionManager()
{

}

template<typename _T, typename _N>
bool CollisionManager::CheckCollide(_T R, _N N)
{
	return false;
}
template<>
bool CollisionManager::CheckCollide(std::shared_ptr<Sphere> Sph0, std::shared_ptr<Sphere> SPh1)
{
	return true;
}