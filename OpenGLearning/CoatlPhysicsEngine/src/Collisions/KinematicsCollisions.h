#pragma once
#include "BaseCols.h"
#include "../Maths/Maths.h"
#include "../Physics/Phys_Libs.h"
namespace CoatlPhysicsEngine {
	class KinematicsCollisions : public BaseCols
	{
	private:
		std::unique_ptr<Queries> AlgoCheck;
		std::weak_ptr<Terrain> Ter;
		float Ext;
		float B_Ex;
		Alg_Type AlgoType;
	public:
		KinematicsCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols);
		~KinematicsCollisions();
		std::vector<std::shared_ptr<Bodies>> GetBods(std::shared_ptr<Bodies> Bod);
		void UpdateBodies(float dt);
	};
}

