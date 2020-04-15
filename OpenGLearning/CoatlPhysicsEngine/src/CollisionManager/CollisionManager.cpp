#include "CollisionManager.h"
using namespace CoatlPhysicsEngine;

CollisionManager::CollisionManager()
	:SphereColSphere(),AABBColAABB(),CapsuleColCapsule(),
        SphereColAABB(), CapsuleColSphere(), CapsuleColAABB(),TriangleColSphere(),
    TriangleColCapsule()
{}
CollisionManager::~CollisionManager()
{}
///Base Collisions
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
 ///Same Object Collisions
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
 template<>
 bool CollisionManager::CheckCollide(Capsule Cap0,Capsule Cap1)
 {
     return CapsuleCols(Cap0, Cap1);
 }
///Different Combinations
 template<>
 bool CollisionManager::CheckCollide(Sphere Sph0,AABB_Obj Obj)
 {
     return SphereColsAABB(Sph0, Obj);
 }
 template<>
 bool CollisionManager::CheckCollide(AABB_Obj obj, Sphere Sph0)
 {
     return SphereColsAABB(Sph0, obj);
 }

 template<>
 bool CollisionManager::CheckCollide(Capsule Cap, Sphere Sph)
 {
     return Collision(Cap,Sph);
 }
 template<>
 bool CollisionManager::CheckCollide(Sphere Sph,Capsule Cap)
 {
     return Collision(Cap, Sph);
 }

 template<>
 bool CollisionManager::CheckCollide(Capsule Cap, AABB_Obj Obj)
 {
     return CapColAABB(Cap,Obj);
 }
 template<>
 bool CollisionManager::CheckCollide(AABB_Obj Obj, Capsule Cap)
 {
     return CapColAABB(Cap, Obj);
 }

 template<>
 bool CollisionManager::CheckCollide(Triangles Tr, Sphere Sph)
 {
     return TrColSphere(Tr, Sph);
 }
 template<>
 bool CollisionManager::CheckCollide(Sphere Sph, Triangles Tr)
 {
     return TrColSphere(Tr, Sph);
 }

 template<>
 bool CollisionManager::CheckCollide(Triangles Tr, Capsule Cap)
 {
     return TrColCap(Tr, Cap);
 }
 template<>
 bool CollisionManager::CheckCollide(Capsule Cap, Triangles Tr)
 {
     return TrColCap(Tr, Cap);
 }