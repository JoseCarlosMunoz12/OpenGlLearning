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

void QuadTree::SeperateBods()
{
	for (auto& jj : Bods)
	{
		this->Insert(jj);
	}
	Bods.clear();
}

bool QuadTree::InsidePar(glm::vec3 Pos)
{
	if (Pos.x > (Center.x + Ext) || Pos.x < (Center.x - Ext))
		return false;
	if (Pos.y > (Center.y + Ext) || Pos.y < (Center.y - Ext))
		return false;
	return true;
}

bool CoatlPhysicsEngine::QuadTree::InsidePar(glm::vec3 Pos, float Ex)
{
	if (glm::abs(Pos.x - Center.x) >= (Ext + Ex))
		return false;
	if (glm::abs(Pos.y - Center.y) >= (Ext + Ex))
		return false;
	return true;
}

QuadTree::QuadTree(glm::vec3 Pos, float Dim,int QuadSize)
	:Queries()
{
	this->QT_Cap = QuadSize;
	this->Center = Pos;
	this->Ext = Dim;
}

QuadTree::~QuadTree()
{
}

bool QuadTree::Insert(std::shared_ptr<Bodies> Bod)
{
	glm::vec3 Pos = Bod->GetPos();
	//check if points can be inside
	if (!InsidePar(Pos))
	{
		return false;
	}
	int Size = Bods.size();
	//See if there is room in QuadTree
	if (Size < QT_Cap && NorthWest == NULL )
	{
		Bods.push_back(Bod);
		return true;
	}
	//Determine to
	if (NorthWest == NULL)
	{
		this->InitChilds();
		this->SeperateBods();
	}
	//Check which to insert
	if (NorthWest->Insert(Bod)) return true;
	if (NorthEast->Insert(Bod)) return true;
	if (SouthWest->Insert(Bod)) return true;
	if (SouthEast->Insert(Bod)) return true;
	//this happens if it fails.
	return false;
}
std::vector<std::shared_ptr<Bodies>> QuadTree::GetQueries(std::shared_ptr<Bodies> Bod, float Ext)
{
	std::vector<std::shared_ptr<Bodies>> Temp;
	glm::vec3 Loc = Bod->GetPos();
	//Check if the Location and Ext is in the Quadtree
	if (!InsidePar(Loc,Ext))
		return Temp;
	//Append all Bods in the Quad
	for (auto& jj : Bods)
	{
		if (jj->GetID() != Bod->GetID())
			Temp.push_back(jj);
	}
	//Terminate if there is no Children
	if (NorthWest == NULL)
		return Temp;
	//get rest of Bodies
	std::vector<std::shared_ptr<Bodies>> NWBods = NorthWest->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), NWBods.begin(), NWBods.end());				
																		
	std::vector<std::shared_ptr<Bodies>> NEBods = NorthEast->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), NEBods.begin(), NEBods.end());				
																		
	std::vector<std::shared_ptr<Bodies>> SWBods = SouthWest->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), SWBods.begin(), SWBods.end());				
																		
	std::vector<std::shared_ptr<Bodies>> SEBods = SouthEast->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), SEBods.begin(), SEBods.end());
	return Temp;
}

std::string CoatlPhysicsEngine::QuadTree::GetAlgorType()
{
	return "QuadTree";
}

void QuadTree::Erase()
{
	this->Bods.clear();
}
