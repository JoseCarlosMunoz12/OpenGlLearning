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
#include "../CollisionResolution/Col_Resolution.h"
#include "Terrain.h"
#include "../OptimizaAlgorithm/QuadTree.h"
#include "../OptimizaAlgorithm/OctoTree.h"
#include "../OptimizaAlgorithm/B_Force.h"
#include "../OptimizaAlgorithm/B_Force_Self.h"
#include "../GJK/GJK.h"

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
		std::shared_ptr<CollisionManager> ColMan;
		std::string Name;
		std::vector<std::shared_ptr<Bodies>> AllBods;
		std::unique_ptr<Queries> AlgoCheck;
		std::weak_ptr<Terrain> Ter;
		std::unique_ptr<GJK_Alg> S;
		int NewCurID;
		std::vector<std::shared_ptr<Manifold>> ColRel;
		bool ColBods(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
	public:
		BaseCols(std::string Name, std::shared_ptr<CollisionManager> InitCols);
		~BaseCols();
		void SetTerrain(std::shared_ptr<Terrain> NewTer);
	};
}
