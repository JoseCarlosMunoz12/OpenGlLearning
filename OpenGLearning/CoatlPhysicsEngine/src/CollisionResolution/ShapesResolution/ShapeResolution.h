#pragma once
#include "../Contact.h"
#include "SphereRelSphere.h"
#include "CapsuleRelSphere.h"
#include "CapsuleRelCapsule.h"
namespace CoatlPhysicsEngine
{
	class ShapeResolution : public SphereRelSphere,CapsuleRelSphere,CapsuleRelCapsule
	{
	public:
		ShapeResolution();
		~ShapeResolution();
		template<typename _T, typename _N>
		std::vector<std::shared_ptr<Contact>> GetContacts(_T T,_N N);
	};


}