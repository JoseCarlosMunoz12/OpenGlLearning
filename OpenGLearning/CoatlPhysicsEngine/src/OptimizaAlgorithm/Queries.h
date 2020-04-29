#pragma once
#include "../Bodies/Bodies.h"
namespace CoatlPhysicsEngine {
	class Queries {
	public:
		Queries() {}
		~Queries() {}
		virtual bool Insert(std::shared_ptr<Bodies> Bod)=0;
		virtual std::vector<Bodies> GetQueries(glm::vec3 Loc, float Ext) =0;
	};
}