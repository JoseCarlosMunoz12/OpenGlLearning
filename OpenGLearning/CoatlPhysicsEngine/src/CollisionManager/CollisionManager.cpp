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
static bool CollisionManager::CheckCollide(_T R, _N N)
{
	return false;
}
template<>
static bool CollisionManager::CheckCollide(Sphere Sph0,Sphere SPh1)
{
	return true;
}