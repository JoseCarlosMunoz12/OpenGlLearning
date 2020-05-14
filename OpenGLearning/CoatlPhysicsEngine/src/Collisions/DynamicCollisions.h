#pragma once
#include "BaseCols.h"
#include "StaticCollisions.h"
#include "../Physics/Phy_Col.h"
#include "../Physics/Phy_Motion.h"
#include "../Maths/Maths.h"

namespace CoatlPhysicsEngine {
	class DynamicCollisions : public BaseCols, Phy_Col,Phy_Motion
	{
	private:
		std::unique_ptr<Queries> AlgoCheck;
		std::weak_ptr<Terrain> Ter;
		int NewCurId = 100;
		float Ext;
		float B_Ex;
		Alg_Type AlgoType;
		const float EPSILON = 0.00017;
		bool BinColDetection(std::shared_ptr<Bodies> Bod0,std::shared_ptr<Bodies> Bod1,
			glm::vec3 Vel,glm::vec3 InitPos,
			float t0, float t1,
			float &NewDt );
		void CalcPhysics(std::weak_ptr<Bodies> Bod0, std::weak_ptr<Bodies>Bod1);
	public:
		DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols);
		~DynamicCollisions();
		void CheckCollision(std::shared_ptr<StaticCollisions> Statics,glm::vec3 Grav,float dt);
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
