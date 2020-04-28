#pragma once
#include "Queries.h"
namespace CoatlPhysicsEngine {
	class QuadTree : public Queries
	{
	private:
		//Holds Bodies in Quadtree part
		std::vector<std::shared_ptr<Bodies>> Bods;
		//Children QuadTree
		std::unique_ptr<QuadTree> NorthWest;
		std::unique_ptr<QuadTree> NorthEast;
		std::unique_ptr<QuadTree> SouthWest;
		std::unique_ptr<QuadTree> SouthEast;
		//Quadtree Parameters
		AABB_Obj Parameters;
		const int QT_CAp = 4;
		void InitChilds();
	public:
		QuadTree(AABB_Obj Range);
		~QuadTree();
		void Insert(std::shared_ptr<Bodies> Bod) override;
		std::vector<Bodies> GetQueries(AABB_Obj ABRange) override;
	};
}