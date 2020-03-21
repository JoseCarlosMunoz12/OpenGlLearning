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
		static bool Collided(_T R,_N S);
	};
	template<typename _T, typename _N>
	static bool CollisionManager::Collided(_T R, _N S)
	{
		return false;
	}
	 template<>
	 static bool CollisionManager::Collided(Sphere Sph0, Sphere Sph1)
	 {
		 return true;
	 }
}

