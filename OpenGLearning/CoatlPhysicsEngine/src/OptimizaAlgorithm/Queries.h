#pragma once
#include "../Bodies/Bodies.h"
namespace CoatlPhysicsEngine {
	class Queries {
	public:
		Queries() {}
		~Queries() {}
		virtual bool Insert(std::shared_ptr<Bodies> Bod)=0;
		virtual std::vector<std::shared_ptr<Bodies>> GetQueries(std::shared_ptr<Bodies> Bod, float Ext) =0;
		virtual std::string GetAlgorType()
		{
			return "None";
		}
		virtual void Erase() {};
	};
}