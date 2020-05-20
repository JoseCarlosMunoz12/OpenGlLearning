#pragma once
#include "BaseCols.h"

namespace CoatlPhysicsEngine 
{	
	class StaticCollisions : public BaseCols
	{
	private:
		std::unique_ptr<Queries> AlgoCheck;
		std::weak_ptr<Terrain> Ter;
		int NewCurId = 0;
		float Ext;
		float B_Ex;
		Alg_Type AlgoType;
	public:
		StaticCollisions(std::string Name,std::shared_ptr<CollisionManager>InitCols);
		~StaticCollisions();
		void UpdateCollisionCheck();
		//Use this Function to check collision from Dynamic Objects
		void CheckCol(std::shared_ptr<Bodies>Bod);
		void AddNewBody(std::shared_ptr<ColShapes> NewShape);
		void AddNewBody(std::vector<std::shared_ptr<ColShapes>> NewShapes);
		std::string GetName();
		std::vector<std::weak_ptr<Bodies>> GetAllBodies();
		std::vector<std::shared_ptr<Bodies>> GetBods(std::shared_ptr<Bodies>Bod);
		std::shared_ptr<Bodies> GetABody(int ID);
		//change and check type of CollisionCheckAlgoritm
		Alg_Type GetType();
		void SetNewType(Alg_Type NewType);
		void SetTerrain(std::shared_ptr<Terrain> NewTer);
	};
}

