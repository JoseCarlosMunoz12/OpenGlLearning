#include "CapsuleColSphere.h"
using namespace CoatlPhysicsEngine;

bool CapsuleColSphere::Collision(Capsule Cap0, Sphere Sph0)
{
	float Sph_R = Sph0.GetRadius();
	float Cp_R = Cap0.GetRadius();

	glm::vec3 T = MATH::ClosestPoint_Seg(Cap0.GetSegment(), Sph0.GetPos());
	float DisSqr = glm::distance(Sph0.GetPos(), T);
	float R_Sqr = Sph_R + Cp_R;

	return R_Sqr > DisSqr;
}

CapsuleColSphere::CapsuleColSphere()
{
}
