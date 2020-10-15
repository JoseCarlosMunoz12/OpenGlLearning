#pragma once
#include "../Contact.h"
#include "../../Shapes/Capsule.h"
#include "../../Shapes/Sphere.h"
namespace CoatlPhysicsEngine
{
	class CapsuleRelSphere
	{
	protected:
		std::vector<std::shared_ptr<Contact>> CapRelSph(Capsule Cap, Sphere Sph0);
	public:
		CapsuleRelSphere();
		~CapsuleRelSphere();
	};

}