#include "CapsuleColSphere.h"
using namespace CoatlPhysicsEngine;

bool CoatlPhysicsEngine::CapsuleColSphere::Collision(Capsule Cap0, Sphere Sph0)
{
	float Sph_R = Sph0.GetRadius();
	float Cp_R = Cap0.GetRadius();

	float DisSqr = Cap0.Distance(Sph0.GetPos());
	float R_Sqr = Sph_R * Sph_R + Cp_R * Cp_R;

	return R_Sqr > DisSqr;
}

CoatlPhysicsEngine::CapsuleColSphere::CapsuleColSphere()
{
}
