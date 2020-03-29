#include "CollisionManager.h"
using namespace CoatlPhysicsEngine;

CollisionManager::CollisionManager()
	:SphereColSphere(),AABBColAABB(),SphereColAABB()
{}

CollisionManager::~CollisionManager()
{}

template<typename _T, typename _N>
 bool CollisionManager::CheckCollide(_T R, _N N)
{
	return false;
}
template<>
bool CollisionManager::CheckCollide(ColShapes Sph0, ColShapes SPh1)
{
	return true;
}
 //Same Object Collisions
template<>
bool CollisionManager::CheckCollide(Sphere Sph0,Sphere SPh1)
{
	return SphereSphere(Sph0,SPh1);
}
 template<>
 bool CollisionManager::CheckCollide(AABB_Obj Obj0, AABB_Obj Obj1)
 {
     return AABBAABB(Obj0, Obj1);
 }
//Different Combinations
 template<>
 bool CollisionManager::CheckCollide(Sphere Sph0,AABB_Obj Obj)
 {
     return SphereColsAABB(Sph0, Obj);
 }
 template<>
 bool CollisionManager::CheckCollide(AABB_Obj obj, Sphere Sph0)
 {
     return CheckCollide(Sph0, obj);
 }