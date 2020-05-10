#pragma once
#include "BaseCols.h"
#include "StaticCollisions.h"

namespace CoatlPhysicsEngine {
	class DynamicCollisions : public BaseCols
	{
	private:
		std::unique_ptr<Queries> AlgoCheck;
		std::weak_ptr<Terrain> Ter;
		int NewCurId = 100;
		float Ext;
		float B_Ex;
		Alg_Type AlgoType;
	public:
		DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols);
		~DynamicCollisions();
		void CheckCollision(std::shared_ptr<StaticCollisions> Statics);
		void UpdatePhysics(float dt);
		//Body information of Class
		void AddNewBody(std::shared_ptr<ColShapes> NewShape);
		void AddNewBody(std::vector<std::shared_ptr<ColShapes>> NewShapes);
		std::string GetName();
		std::vector<std::weak_ptr<Bodies>> GetAllBodies();
		std::shared_ptr<Bodies> GetABody(int ID);
		//change and check type of CollisionCheckAlgoritm
		Alg_Type GetType();
		void SetNewType(Alg_Type NewType);
		void SetTerrain(std::shared_ptr<Terrain> NewTer);
	};
}
