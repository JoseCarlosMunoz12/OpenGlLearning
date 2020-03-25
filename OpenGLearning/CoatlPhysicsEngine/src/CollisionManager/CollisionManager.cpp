#include "CollisionManager.h"
using namespace CoatlPhysicsEngine;

CollisionManager::CollisionManager()
	:SphereColSphere(),AABBColAABB()
{}

CollisionManager::~CollisionManager()
{}

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
 template<>
 bool CollisionManager::CheckCollide(AABBColAABB Obj0, AABBColAABB Obj1)
 {
     return true;
 }