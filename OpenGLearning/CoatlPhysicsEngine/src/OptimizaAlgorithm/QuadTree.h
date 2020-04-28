#pragma once
#include "Queries.h"
namespace CoatlPhysicsEngine {
	class QuadTree : public Queries
	{
		std::vector<std::shared_ptr<Bodies>> Bods;
		std::unique_ptr<QuadTree> NorthEast;
	public:
		QuadTree(AABB_Obj Range);
		~QuadTree();
		void Insert(std::shared_ptr<Bodies> Bod) override;
		std::vector<Bodies> GetQueries(AABB_Obj ABRange) override;
	};
}