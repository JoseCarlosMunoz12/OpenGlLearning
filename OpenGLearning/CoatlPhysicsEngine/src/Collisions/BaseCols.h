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
#include "../Bodies/Bod_Collection.h"
#include "../Bodies/Joints/Joints.h"
#include "../CollisionManager/CollisionManager.h"
#include "../CollisionResolution/Col_Resolution.h"
#include "Terrain.h"
#include "../OptimizaAlgorithm/QuadTree.h"
#include "../OptimizaAlgorithm/OctoTree.h"
#include "../OptimizaAlgorithm/B_Force.h"
#include "../OptimizaAlgorithm/B_Force_Self.h"
#include "../GJK/GJK.h"
#include "../CollisionResolution/ContactCreation.h"

namespace CoatlPhysicsEngine {
	enum class Alg_Type
	{
		B_F,
		Q_T,
		O_T
	};
	class BaseCols
	{
	private:
		bool UpdateBodies(Sphere Sph0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		bool UpdateBodies(Capsule Cap0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
	protected:
		//name
		std::string Name;
		// bodies in the collision world and Collections in this world
		std::vector<std::shared_ptr<Bodies>> AllBods;
		std::vector<std::shared_ptr<Collection>> AllCollections;
		//Algoirthm to check collision
		std::shared_ptr<CollisionManager> ColMan;
		std::unique_ptr<Queries> AlgoCheck;
		std::weak_ptr<Terrain> Ter;
		std::unique_ptr<GJK_Alg> S;
		std::unique_ptr<ContactCreation> ContCrt;
		int NewCurID;
		//collision manifolds
		std::vector<std::shared_ptr<Contact>> ColRel;
		// other functions
		bool ColBods(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		bool ColBods(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1, std::vector<glm::vec3> Seg0, std::vector<glm::vec3> S);
	public:
		BaseCols(std::string Name, std::shared_ptr<CollisionManager> InitCols);
		~BaseCols();
		std::vector<std::shared_ptr<Contact>> GetColRel();
		void SetTerrain(std::shared_ptr<Terrain> NewTer);
		std::vector<std::shared_ptr<Collection>> GetCollections();
	};
}
