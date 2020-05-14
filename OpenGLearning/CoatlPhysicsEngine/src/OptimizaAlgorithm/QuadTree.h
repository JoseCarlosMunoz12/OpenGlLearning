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
		glm::vec3 Center;
		float Ext;
		int QT_Cap;
		void InitChilds();
		void SeperateBods();
		bool InsidePar(glm::vec3 Pos);
		bool InsidePar(glm::vec3 Pos, float Ex);
	public:
		QuadTree(glm::vec3 Pos, float Dim,int QuadSize = 4);
		~QuadTree();
		bool Insert(std::shared_ptr<Bodies> Bod) override;
		std::vector<std::shared_ptr<Bodies>> GetQueries(std::shared_ptr<Bodies> Bod, float Ext) override;
		std::string GetAlgorType() override;
		void Erase() override;
	};
}