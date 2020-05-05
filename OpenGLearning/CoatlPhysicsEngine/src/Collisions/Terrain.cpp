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
		int Tr_V0 = ii * 3;
		int Tr_V1 = Tr_V0 + 1;
		int Tr_V2 = Tr_V1 + 1;
		std::vector<glm::vec3> Loc;
		Loc.push_back(Mesh[Tr_V0] - Mesh[Tr_V0]);
		Loc.push_back(Mesh[Tr_V1] - Mesh[Tr_V0]);
		Loc.push_back(Mesh[Tr_V2] - Mesh[Tr_V0]);
		std::shared_ptr<Triangles> Tr = std::make_shared<Triangles>(glm::vec3(Mesh[Tr_V0]), 				Loc);
		std::shared_ptr<Bodies> Bod = std::make_shared<Bodies>(Count,glm::vec3(0.f));
		Bod->AddShapes(Tr);
		TerWorld->Insert(Bod);
	}
}

Terrain::~Terrain()
{
}

std::vector<std::shared_ptr<Bodies>> Terrain::GetTerrs(glm::vec3 Pos, float Ext)
{
	std::shared_ptr<Bodies> Bod = std::make_shared<Bodies>(-1);
	return TerWorld->GetQueries(Bod,Ext);
}
