#include "CapsuleColCapsule.h"
using namespace CoatlPhysicsEngine;

bool CapsuleColCapsule::CapsuleCols(Capsule Cap0, Capsule Cap1)
{
	float Cap0R = Cap0.GetRadius();
	float Cap1R = Cap1.GetRadius();
	float RDis = Cap0R + Cap1R;

	float Dis = Cap0.Distance(Cap1.GetSegment());

	return RDis > Dis;
}

CapsuleColCapsule::CapsuleColCapsule()
{
}
