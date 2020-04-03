#pragma once
#include "../Shapes/Capsule.h"
#include "../Shapes/Sphere.h"

namespace CoatlPhysicsEngine{
class CapsuleColSphere
{
protected:
	bool Collision(Capsule Cap0,Sphere Sph0);
public:
	CapsuleColSphere();
};
}
