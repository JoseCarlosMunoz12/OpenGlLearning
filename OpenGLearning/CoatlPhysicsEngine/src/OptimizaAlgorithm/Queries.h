#pragma once
#include "../Shapes/AABB_Obj.h"
#include "../Bodies/Bodies.h"
namespace CoatlPhysicsEngine {
	class Queries {
	public:
		Queries() {}
		~Queries() {}
		virtual bool Insert(std::shared_ptr<Bodies> Bod)=0;
		virtual std::vector<Bodies> GetQueries(AABB_Obj ABRange) =0;
	};
}