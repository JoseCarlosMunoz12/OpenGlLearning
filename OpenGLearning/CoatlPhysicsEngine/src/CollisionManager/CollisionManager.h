#pragma once
#include "SphereColSphere.h"

namespace CoatlPhysicsEngine {
	class CollisionManager : public SphereColSphere
	{
	private:
	public:		
		CollisionManager();
		~CollisionManager();
		template<typename _T,typename _N>
		bool CheckCollide(_T R,_N N);
	};
}

