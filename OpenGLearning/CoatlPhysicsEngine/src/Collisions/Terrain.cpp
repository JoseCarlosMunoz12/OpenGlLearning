#include "Terrain.h"
using namespace CoatlPhysicsEngine;

Terrain::Terrain(std::vector<glm::vec3> Mesh, std::vector<int> Index,float Dim)
{
	int Size = Index.size();
	int Range = Size / 3 / 2;
	this->TerWorld = std::make_unique<OctoTree>(glm::vec3(0.f),Dim);
	int Count = 0;
	for (int ii = 0; ii < Range; ii++)
	{
		for (int jj = 1; jj <= 2; jj++)
		{
			std::shared_ptr<Bodies> Bod = std::make_shared<Bodies>(Count);
			std::shared_ptr<Triangles> Tr;
			Bod->AddShapes(Tr);
			Count++;
		}
	}
}

Terrain::~Terrain()
{
}

std::vector<std::shared_ptr<Bodies>> Terrain::GetTerrs(glm::vec3 Pos, float Ext)
{
	return std::vector<std::shared_ptr<Bodies>>();
}
