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

namespace CoatlPhysicsEngine 
{
	class StaticCollisions
	{
	private:
		std::shared_ptr<CollisionManager> ColMan;
		std::string Name;
		std::vector<std::shared_ptr<Bodies>> AllStatics;
		int NewCurId = 0;
	protected:
		bool UpdateBodies(Sphere Bod0, std::shared_ptr<Bodies> Bod1);
		bool UpdateBodies(AABB_Obj Obj0, std::shared_ptr<Bodies> Bod1);
	public:
		StaticCollisions(std::string Name,std::shared_ptr<CollisionManager>InitCols);
		~StaticCollisions();
		void UpdateCollisionCheck();
		void AddNewBody(glm::vec3 InitPos,std::shared_ptr<ColShapes> NewShape);
		void AddNewBody(glm::vec3 InitPos,std::vector<std::shared_ptr<ColShapes>> NewShapes);
		std::string GetName();
		std::vector<std::shared_ptr<Bodies>> GetAllBodies();
		std::shared_ptr<Bodies> GetABody(int ID);
	};
}

