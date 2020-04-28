#include "QuadTree.h"
using namespace CoatlPhysicsEngine;

void QuadTree::InitChilds()
{
	glm::vec3 Pos = Parameters.GetPos();
	float Dim = Parameters.GetEx().x;
	this->NorthWest = std::make_unique<QuadTree>(new AABB_Obj(glm::vec3(Pos.x - Dim / 2, Pos.y + Dim / 2, Pos.z), Dim / 2, Dim / 2, 100.f));
	this->NorthEast = std::make_unique<QuadTree>(new AABB_Obj(glm::vec3(Pos.x + Dim / 2, Pos.y + Dim / 2, Pos.z), Dim / 2, Dim / 2, 100.f));
	this->SouthWest = std::make_unique<QuadTree>(new AABB_Obj(glm::vec3(Pos.x - Dim / 2, Pos.y - Dim / 2, Pos.z), Dim / 2, Dim / 2, 100.f));
	this->SouthEast = std::make_unique<QuadTree>(new AABB_Obj(glm::vec3(Pos.x + Dim / 2, Pos.y - Dim / 2, Pos.z), Dim / 2, Dim / 2, 100.f));
}

QuadTree::QuadTree(AABB_Obj Range)
	:Parameters(Range)
{
}

QuadTree::~QuadTree()
{
}

bool QuadTree::Insert(std::shared_ptr<Bodies> Bod)
{
	glm::vec3 Pos = Bod->GetPos();
	if (!Parameters.Inside(Pos))
	{
		return false;
	}
	if (Bods.size() < QT_Cap && NorthWest)
	{
		Bods.push_back(Bod);
		return true;
	}

}
std::vector<Bodies> QuadTree::GetQueries(AABB_Obj ABRange)
{
	return std::vector<Bodies>();
}
