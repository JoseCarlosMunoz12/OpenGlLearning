#include "QuadTree.h"
using namespace CoatlPhysicsEngine;

void QuadTree::InitChilds()
{
	glm::vec3 Pos = Parameters.GetPos();
	float Dim = Parameters.GetEx().x;
	this->NorthWest = std::make_unique<QuadTree>(new AABB_Obj(glm::vec3(Pos.x - Dim / 2, Pos.y + Dim / 2, Pos.z),Dim/2, Dim / 2, 10.f));
	this->NorthEast = std::make_unique<QuadTree>(new AABB_Obj(glm::vec3(Pos.x + Dim / 2, Pos.y + Dim / 2, Pos.z), Dim / 2, Dim / 2, 10.f));
	this->SouthWest = std::make_unique<QuadTree>(new AABB_Obj(glm::vec3(Pos.x - Dim / 2, Pos.y - Dim / 2, Pos.z), Dim / 2, Dim / 2, 10.f));
	this->SouthEast = std::make_unique<QuadTree>(new AABB_Obj(glm::vec3(Pos.x + Dim / 2, Pos.y - Dim / 2, Pos.z), Dim / 2, Dim / 2, 10.f));
}

QuadTree::QuadTree(AABB_Obj Range)
	:Parameters(Range)
{
}

QuadTree::~QuadTree()
{
}

void QuadTree::Insert(std::shared_ptr<Bodies> Bod)
{
	
}
std::vector<Bodies> QuadTree::GetQueries(AABB_Obj ABRange)
{
	return std::vector<Bodies>();
}
