#pragma once
#include "SphereColSphere.h"
#include "AABBColAABB.h"
#include "CapsuleColCapsule.h"

#include "SphereColAABB.h"
#include "CapsuleColSphere.h"
#include "CapsuleColAABB.h"

#include "TriangleColSphere.h"
#include "TriangleColCapsule.h"
#include "TriangleColAABB.h"

namespace CoatlPhysicsEngine {
	class CollisionManager
		: public SphereColSphere,AABBColAABB,CapsuleColCapsule,
		 SphereColAABB, CapsuleColSphere, CapsuleColAABB,
		TriangleColSphere,TriangleColCapsule,TriangleColAABB
	{
	private:
	public:		
		CollisionManager();
		~CollisionManager();
		template<typename _T,typename _N>
		bool CheckCollide(_T R,_N N);
	};
}

