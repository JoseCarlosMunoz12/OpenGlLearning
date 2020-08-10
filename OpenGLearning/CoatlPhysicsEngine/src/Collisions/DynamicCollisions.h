#pragma once
#include "BaseCols.h"
#include "StaticCollisions.h"
#include "../Maths/Maths.h"
#include "../Physics/Phys_Libs.h"

namespace CoatlPhysicsEngine {
	class DynamicCollisions : public BaseCols
	{
	private:
		std::unique_ptr<Phy_Base> F_Manager;
		std::unique_ptr<Phy_Base> Grav_F_Manager;
		std::unique_ptr<Queries> AlgoCheck;
		std::unique_ptr<Col_Resolution> Col_Rel;
		std::weak_ptr<Terrain> Ter;
		glm::vec3 Gravity;
		int NewCurId = 100;
		float Ext;
		float B_Ex;
		Alg_Type AlgoType;
		const float EPSILON = 0.000017f;
		bool BinColDetection(std::shared_ptr<Bodies> Bod0,std::shared_ptr<Bodies> Bod1,
			glm::vec3 Vel,
			float t0, float t1,
			float &NewDt );
		bool BinColDetection(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1,
			glm::vec3 Vel0, glm::vec3 Vel1,
			float t0, float t1,
			float& NewDt);
		void CalcPhysics(std::weak_ptr<Bodies> Bod0, std::weak_ptr<Bodies>Bod1);
		bool ContainsManifold(std::vector<std::shared_ptr<Manifold>> ColRel, std::shared_ptr<Manifold> NewMan);
	public:
		DynamicCollisions(std::string Name, std::shared_ptr<CollisionManager>InitCols);
		~DynamicCollisions();
		void CheckCollision(std::shared_ptr<StaticCollisions> Statics,float dt);
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
