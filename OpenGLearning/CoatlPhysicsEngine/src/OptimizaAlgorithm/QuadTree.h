#pragma once
#include "Queries.h"
#include "../Shapes/OBB.h"
namespace CoatlPhysicsEngine {
	class QuadTree
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
		glm::vec3 Center;
		float Ext;
		const int QT_Cap = 4;
		void InitChilds();
		void SeperateBods();
		bool InsidePar(glm::vec3 Pos);
	public:
		QuadTree(glm::vec3 Pos, float Dim);
		~QuadTree();
		bool Insert(std::shared_ptr<Bodies> Bod);
		std::vector<Bodies> GetQueries(glm::vec3 Loc, float Ext);
	};
}