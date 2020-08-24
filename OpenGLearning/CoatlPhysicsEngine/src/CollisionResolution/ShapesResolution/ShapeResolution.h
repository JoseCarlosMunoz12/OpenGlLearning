#pragma once
#include "../Col_Resolution.h"
#include "SphereRelSphere.h"
#include "SphereRelAABB.h"
#include "AABBRelAABB.h"
#include "CapsuleRelSphere.h"
#include "CapsuleRelAABB.h"
#include "CapsuleRelCapsule.h"
namespace CoatlPhysicsEngine
{
	class ShapeResolution : public SphereRelSphere,SphereRelAABB,AABBRelAABB,CapsuleRelSphere,CapsuleRelAABB,CapsuleRelCapsule
	{
	public:
		ShapeResolution();
		~ShapeResolution();
		template<typename _T, typename _N>
		std::vector<std::shared_ptr<Contacts>> GetContacts(_T T,_N N);
	};


}