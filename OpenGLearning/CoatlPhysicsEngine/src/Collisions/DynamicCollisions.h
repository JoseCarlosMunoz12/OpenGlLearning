#pragma once
#include "BaseCols.h"
#include "StaticCollisions.h"

namespace CoatlPhysicsEngine {
	class DynamicCollisions : public BaseCols
	{
	private:
		std::unique_ptr<Queries> AlgoCheck;
		std::weak_ptr<Terrain> Ter;
		int NewCurId = 0;
		float Ext;
		float B_Ex;
		Alg_Type AlgoType;
	public:
		DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols);
		~DynamicCollisions();
		void CheckCollision(std::shared_ptr<StaticCollisions> Statics);
		std::vector<std::shared_ptr<Bodies>> GetBodies();
	};
}
