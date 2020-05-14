#pragma once
#include "Queries.h"
namespace CoatlPhysicsEngine {
	class OctoTree : public Queries
	{
	private:
		///Holds Bodies in OctoTree
		std::vector<std::shared_ptr<Bodies>> Bods;
		///Childrens of Tree

		///OctoTree Parameters
		glm::vec3 Center;
		float Ext;
		int OT_Cap;
		//Top Boxes
		std::unique_ptr<OctoTree> Box0;
		std::unique_ptr<OctoTree> Box1;
		std::unique_ptr<OctoTree> Box2;
		std::unique_ptr<OctoTree> Box3;
		//Bottome Boxes
		std::unique_ptr<OctoTree> Box4;
		std::unique_ptr<OctoTree> Box5;
		std::unique_ptr<OctoTree> Box6;
		std::unique_ptr<OctoTree> Box7;
		///Functions in OctoTree
		void InitChilds();
		void SeperateBods();
		bool InsidePar(glm::vec3 Pos);
		bool InsidePar(glm::vec3 Pos,float Ex);
	public:
		OctoTree(glm::vec3 InitCent, float Ex, int Cap = 8);
		~OctoTree();
		bool Insert(std::shared_ptr<Bodies> Bod) override;
		std::vector<std::shared_ptr<Bodies>> GetQueries(std::shared_ptr<Bodies> Bod, float Ext) override;
		std::vector<std::shared_ptr<Bodies>> GetQueries(glm::vec3 Pos, float Ext);
		std::string GetAlgorType() override;
		void Erase() override;
	};
}