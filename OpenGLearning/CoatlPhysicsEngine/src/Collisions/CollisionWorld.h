#pragma once
#include <string>
#include <vector>
#include "DynamicCollisions.h"
#include "StaticCollisions.h"
#include "KinematicsCollisions.h"
#include "../CollisionManager/CollisionManager.h"
namespace CoatlPhysicsEngine {
	struct CollisionWorldSetup
	{
		std::string Name;
		glm::vec3 Gravity = glm::vec3(0.f, 0.f, -9.81);
	};
	class CollisionWorld
	{
	private:
		glm::vec3 Gravity;
		std::shared_ptr<StaticCollisions> Statics;
		std::shared_ptr<KinematicsCollisions> Kin;
		std::shared_ptr<DynamicCollisions> Dynamics;
		std::shared_ptr<CollisionManager> ColMan;
		std::string WorldName;
		std::shared_ptr<Terrain> Ter;
		bool CheckStatics = false;
	public:
		CollisionWorld(std::string SetWName);
		CollisionWorld(CollisionWorldSetup SetUp);
		~CollisionWorld();
		std::string ShowName();
		glm::vec3 GetGravity();
		void ChangeName(std::string NewName);
		void CreateStaticCol(std::string Name);
		void CreateDynamicCol(std::string Name);
		void CreateKinCol(std::string Name);
		std::shared_ptr<StaticCollisions> GetCollision();
		std::shared_ptr<DynamicCollisions> GetDynCol();
		std::shared_ptr<KinematicsCollisions> GetKinCol();
		void DeleteStatics();
		void DeleteDynamics();
		void DeleteKinematics();
		void UpdateWorld(float dt);
		void SetTerrain(std::vector<glm::vec3>Ver, std::vector<int>Ind, float Dim);
		void ToggleStaticCheck();
		bool StaticCheckStatus();
	};
}