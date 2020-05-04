#include "Terrain.h"
using namespace CoatlPhysicsEngine;

Terrain::Terrain(std::vector<glm::vec3> Mesh, std::vector<int> Index,int Dim)
{
	int Size = Index.size();
	int Range = Size / 3 * 2;
	this->TerWorld = std::make_unique<OctoTree>(glm::vec3(0.f),(float)Dim);
	for (int ii = 0; ii < Range; ii++)
	{

	}
}

Terrain::~Terrain()
{
}

std::vector<std::shared_ptr<Bodies>> Terrain::GetTerrs(glm::vec3 Pos, float Ext)
{
	return std::vector<std::shared_ptr<Bodies>>();
}
