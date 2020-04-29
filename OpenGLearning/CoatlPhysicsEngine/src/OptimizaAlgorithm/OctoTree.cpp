#include "OctoTree.h"
using namespace CoatlPhysicsEngine;
void OctoTree::InitChilds()
{

}

void OctoTree::SeperateBods()
{
	for (auto& jj : Bods)
	{
		this->Insert(jj);
	}
	Bods.clear();
}

bool OctoTree::InsidePar(glm::vec3 Pos)
{
	if (Pos.x > (Center.x + Ext) || Pos.x < (Center.x - Ext))
		return false;
	if (Pos.y > (Center.y + Ext) || Pos.y < (Center.y - Ext))
		return false;
	if (Pos.z > (Center.z + Ext) || Pos.z < (Center.z - Ext))
		return false;
	return true;
}

bool OctoTree::InsidePar(glm::vec3 Pos, float Ex)
{
	if (glm::abs(Pos.x - Center.x) > (Ext + Ex))
		return false;
	if (glm::abs(Pos.y - Center.y) > (Ext + Ex))
		return false;
	if (glm::abs(Pos.z - Center.z) > (Ext + Ex))
		return false;
	return true;
}

OctoTree::OctoTree(glm::vec3 InitCent, float Ex, int Cap =4)
{
	this->Center = InitCent;
	this->Ext = Ex;
	this->OT_Cap = Cap;
}

OctoTree::~OctoTree()
{
}

bool OctoTree::Insert(std::shared_ptr<Bodies> Bod)
{
	glm::vec3 Pos = Bod->GetPos();
	//check if points can be inside
	if (!InsidePar(Pos))
	{
		return false;
	}
	int Size = Bods.size();
	//See if there is room in QuadTree
	if (Size < OT_Cap && Box0 == NULL)
	{
		Bods.push_back(Bod);
		return true;
	}
	//Determine to
	if (Box0 == NULL)
	{
		this->InitChilds();
		this->SeperateBods();
	}
	//Check which to insert
	if (Box0->Insert(Bod)) return true;
	if (Box1->Insert(Bod)) return true;
	if (Box2->Insert(Bod)) return true;
	if (Box3->Insert(Bod)) return true;
	if (Box4->Insert(Bod)) return true;
	if (Box5->Insert(Bod)) return true;
	if (Box6->Insert(Bod)) return true;
	if (Box7->Insert(Bod)) return true;
	//this happens if it fails.
	return false;
}

std::vector<Bodies> OctoTree::GetQueries(glm::vec3 Loc, float Ext)
{
	std::vector<Bodies> Temp;
	//check if the locatin is in the OctoTre
	if (!InsidePar(Loc,Ext))
		return Temp;
	//Append all Bods in the Quad
	for (auto& jj : Bods)
	{
		Temp.push_back(*jj);
	}
	if (Box0 == NULL)
		return Temp;
	//Get rest of Bodies
	std::vector<Bodies> Bods0 = Box0->GetQueries(Loc, Ext);
	Temp.insert(Temp.end(), Bods0.begin(), Bods0.end());

	std::vector<Bodies> Bods1 = Box1->GetQueries(Loc, Ext);
	Temp.insert(Temp.end(), Bods1.begin(), Bods1.end());

	std::vector<Bodies> Bods2 = Box2->GetQueries(Loc, Ext);
	Temp.insert(Temp.end(), Bods2.begin(), Bods2.end());

	std::vector<Bodies> Bods3 = Box3->GetQueries(Loc, Ext);
	Temp.insert(Temp.end(), Bods3.begin(), Bods3.end());

	std::vector<Bodies> Bods4 = Box4->GetQueries(Loc, Ext);
	Temp.insert(Temp.end(), Bods4.begin(), Bods4.end());

	std::vector<Bodies> Bods5 = Box5->GetQueries(Loc, Ext);
	Temp.insert(Temp.end(), Bods5.begin(), Bods5.end());

	std::vector<Bodies> Bods6 = Box6->GetQueries(Loc, Ext);
	Temp.insert(Temp.end(), Bods6.begin(), Bods6.end());

	std::vector<Bodies> Bods7 = Box7->GetQueries(Loc, Ext);
	Temp.insert(Temp.end(), Bods7.begin(), Bods7.end());

	return Temp;
}
