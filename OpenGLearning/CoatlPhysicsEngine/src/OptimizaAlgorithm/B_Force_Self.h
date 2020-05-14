#pragma once
#include "Queries.h"
namespace CoatlPhysicsEngine {
	class B_Force_Self : public Queries
	{
	private:
		std::vector<std::shared_ptr<Bodies>> Bods;
	public:
		B_Force_Self();
		~B_Force_Self();
		bool Insert(std::shared_ptr<Bodies> Bod) override;
		std::vector<std::shared_ptr<Bodies>> GetQueries(std::shared_ptr<Bodies> Bod, float Ext) override;
		std::string GetAlgorType() override;
		void Erase() override;
	};
}