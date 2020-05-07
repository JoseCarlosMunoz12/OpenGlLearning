#pragma once
#include "BaseCols.h"

namespace CoatlPhysicsEngine {
	class DynamicCollisions : public BaseCols
	{
	private:
		std::vector<ColShapes*> AllShapes;
	public:
		DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols);
		~DynamicCollisions();
	};
}
