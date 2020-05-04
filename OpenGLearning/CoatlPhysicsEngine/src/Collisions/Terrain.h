#pragma once
#include "../OptimizaAlgorithm/OctoTree.h"
namespace CoatlPhysicsEngine
{
	class Terrain
	{
	private:
		std::unique_ptr<OctoTree> TerWorld;
	public:
		Terrain(std::vector<glm::vec3> Mesh, std::vector<int> Index,int Dim);
		~Terrain();
		std::vector<std::shared_ptr<Bodies>> GetTerrs(glm::vec3 Pos, float Ext);
	};

}