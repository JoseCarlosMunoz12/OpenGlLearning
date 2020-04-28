#include "QuadTree.h"
using namespace CoatlPhysicsEngine;

void QuadTree::InitChilds()
{
	glm::vec3 Pos = Center;
	this->NorthWest = std::make_unique<QuadTree>(glm::vec3(Pos.x - Ext / 2, Pos.y + Ext / 2, Pos.z), Ext / 2);
	this->NorthEast = std::make_unique<QuadTree>(glm::vec3(Pos.x + Ext / 2, Pos.y + Ext / 2, Pos.z), Ext / 2);
	this->SouthWest = std::make_unique<QuadTree>(glm::vec3(Pos.x - Ext / 2, Pos.y - Ext / 2, Pos.z), Ext / 2);
	this->SouthEast = std::make_unique<QuadTree>(glm::vec3(Pos.x + Ext / 2, Pos.y - Ext / 2, Pos.z), Ext / 2);
}

QuadTree::QuadTree(glm::vec3 Pos, float Dim)
{
	this->Center = Pos;
	this->Ext = Dim;
}

QuadTree::~QuadTree()
{
}

bool QuadTree::Insert(std::shared_ptr<Bodies> Bod)
{
	glm::vec3 Pos = Bod->GetPos();
	if (!InsiePar(Pos))
	{
		return false;
	}
	int Size = Bods.size();
	if (Size < QT_Cap )
	{
		Bods.push_back(Bod);
		return true;
	}
	if (NorthWest == NULL)
	{
		this->InitChilds();
	}
	if (NorthWest->Insert(Bod)) return true;
	if (NorthEast->Insert(Bod)) return true;
	if (SouthWest->Insert(Bod)) return true;
	if (SouthEast->Insert(Bod)) return true;
	//this happens if it fails.
	return false;
}
bool CoatlPhysicsEngine::QuadTree::InsiePar(glm::vec3 Pos)
{
	return false;
}
std::vector<Bodies> QuadTree::GetQueries(AABB_Obj ABRange)
{
	return std::vector<Bodies>();
}
