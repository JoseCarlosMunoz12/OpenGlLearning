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
 bool CollisionManager::CheckCollide(Sphere Sph0,Sphere SPh1)
{
	return SphereSphere(Sph0,SPh1);
}
template<>
 bool CollisionManager::CheckCollide(ColShapes Sph0, ColShapes SPh1)
{
	return true;
}