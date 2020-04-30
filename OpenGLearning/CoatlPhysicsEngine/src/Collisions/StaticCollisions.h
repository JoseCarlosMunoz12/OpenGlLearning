#pragma once
#include <string>
#include <iostream>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
#include <vector>

#include "../Shapes/ColShapes.h"
#include "../Bodies/Bodies.h"
#include "../CollisionManager/CollisionManager.h"
#include "../OptimizaAlgorithm/QuadTree.h"
#include "../OptimizaAlgorithm/OctoTree.h"
#include "../OptimizaAlgorithm/B_Force.h"

namespace CoatlPhysicsEngine 
{	
	enum class Alg_Type
	{
		B_F,
		Q_T,
		O_T
	};
	class StaticCollisions
	{
	private:
		std::shared_ptr<CollisionManager> ColMan;
		std::string Name;
		std::vector<std::shared_ptr<Bodies>> AllStatics;
		std::unique_ptr<Queries> AlgoCheck;
		int NewCurId = 0;		
	protected:
		float Ext;
		float B_Ex;
		Alg_Type AlgoType;
		bool UpdateBodies(Sphere Bod0, std::shared_ptr<Bodies> Bod1);
		bool UpdateBodies(AABB_Obj Obj0, std::shared_ptr<Bodies> Bod1);
		bool UpdateBodies(Capsule Cap0, std::shared_ptr<Bodies> Bod1);
		bool UpdateBodies(Triangles Tr, std::shared_ptr<Bodies> Bod1);
		bool UpdateBodies(OBB Tr, std::shared_ptr<Bodies> Bod1);
		void ColBods(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
	public:
		StaticCollisions(std::string Name,std::shared_ptr<CollisionManager>InitCols);
		~StaticCollisions();
		void UpdateCollisionCheck();
		void AddNewBody(std::shared_ptr<ColShapes> NewShape);
		void AddNewBody(std::vector<std::shared_ptr<ColShapes>> NewShapes);
		std::string GetName();
		std::vector<std::weak_ptr<Bodies>> GetAllBodies();
		std::shared_ptr<Bodies> GetABody(int ID);
		//change and check type of CollisionCheckAlgoritm
		Alg_Type GetType();
		void SetNewType(Alg_Type NewType);
	};
}

