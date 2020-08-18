#include "Terrain.h"
using namespace CoatlPhysicsEngine;

Terrain::Terrain(std::vector<glm::vec3> Mesh, std::vector<int> Index,float Dim)
{
	int Size = Index.size();
	int Range = Size / 3;
	Dim = Dim / 2.f;
	this->TerWorld = std::make_unique<OctoTree>(glm::vec3(0.f),Dim);
	std::shared_ptr<AABB_Obj> F_World = std::make_shared<AABB_Obj>(glm::vec3(0.f,0.f,-1.f),Dim,Dim,1.f);
	std::shared_ptr<Bodies> Bod = std::make_shared<Bodies>(F_World,-1);
	this->TerWorld->Insert(Bod);
}

Terrain::~Terrain()
{
}

std::vector<std::shared_ptr<Bodies>> Terrain::GetTerrs(glm::vec3 Pos, float Ext)
{
	return TerWorld->GetQueries(Pos,Ext);
}
