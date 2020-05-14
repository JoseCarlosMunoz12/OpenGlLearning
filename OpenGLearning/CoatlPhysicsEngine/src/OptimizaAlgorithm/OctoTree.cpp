#include "OctoTree.h"
using namespace CoatlPhysicsEngine;
void OctoTree::InitChilds()
{
	glm::vec3 Pos = Center;
	//Top Layer
	this->Box0 = std::make_unique<OctoTree>(glm::vec3(Pos.x - Ext / 2, Pos.y + Ext / 2, Pos.z + Ext / 2), Ext / 2);
	this->Box1 = std::make_unique<OctoTree>(glm::vec3(Pos.x + Ext / 2, Pos.y + Ext / 2, Pos.z + Ext / 2), Ext / 2);
	this->Box2 = std::make_unique<OctoTree>(glm::vec3(Pos.x - Ext / 2, Pos.y - Ext / 2, Pos.z + Ext / 2), Ext / 2);
	this->Box3 = std::make_unique<OctoTree>(glm::vec3(Pos.x + Ext / 2, Pos.y - Ext / 2, Pos.z + Ext / 2), Ext / 2);
	//Bottom Layer
	this->Box4 = std::make_unique<OctoTree>(glm::vec3(Pos.x - Ext / 2, Pos.y + Ext / 2, Pos.z - Ext / 2), Ext / 2);
	this->Box5 = std::make_unique<OctoTree>(glm::vec3(Pos.x + Ext / 2, Pos.y + Ext / 2, Pos.z - Ext / 2), Ext / 2);
	this->Box6 = std::make_unique<OctoTree>(glm::vec3(Pos.x - Ext / 2, Pos.y - Ext / 2, Pos.z - Ext / 2), Ext / 2);
	this->Box7 = std::make_unique<OctoTree>(glm::vec3(Pos.x + Ext / 2, Pos.y - Ext / 2, Pos.z - Ext / 2), Ext / 2);
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
	if (glm::abs(Pos.x - Center.x) >= (Ext + Ex))
		return false;
	if (glm::abs(Pos.y - Center.y) >= (Ext + Ex))
		return false;
	if (glm::abs(Pos.z - Center.z) >= (Ext + Ex))
		return false;
	return true;
}

OctoTree::OctoTree(glm::vec3 InitCent, float Ex, int Cap)
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

std::vector<std::shared_ptr<Bodies>> OctoTree::GetQueries(std::shared_ptr<Bodies> Bod, float Ext)
{
	std::vector<std::shared_ptr<Bodies>> Temp;
	//check if the locatin is in the OctoTre
	glm::vec3 Loc = Bod->GetPos();
	if (!InsidePar(Loc,Ext))
		return Temp;
	//Append all Bods in the Octo and not include current Body
	for (auto& jj : Bods)
	{
		if (jj->GetID() !=Bod->GetID())
			Temp.push_back(jj);
	}
	if (Box0 == NULL)
		return Temp;
	//Get rest of Bodies
	std::vector<std::shared_ptr<Bodies>> Bods0 = Box0->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), Bods0.begin(), Bods0.end());

	std::vector<std::shared_ptr<Bodies>> Bods1 = Box1->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), Bods1.begin(), Bods1.end());

	std::vector<std::shared_ptr<Bodies>> Bods2 = Box2->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), Bods2.begin(), Bods2.end());

	std::vector<std::shared_ptr<Bodies>> Bods3 = Box3->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), Bods3.begin(), Bods3.end());

	std::vector<std::shared_ptr<Bodies>> Bods4 = Box4->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), Bods4.begin(), Bods4.end());

	std::vector<std::shared_ptr<Bodies>> Bods5 = Box5->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), Bods5.begin(), Bods5.end());

	std::vector<std::shared_ptr<Bodies>> Bods6 = Box6->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), Bods6.begin(), Bods6.end());

	std::vector<std::shared_ptr<Bodies>> Bods7 = Box7->GetQueries(Bod, Ext);
	Temp.insert(Temp.end(), Bods7.begin(), Bods7.end());
		
	return Temp;
}

std::vector<std::shared_ptr<Bodies>> CoatlPhysicsEngine::OctoTree::GetQueries(glm::vec3 Pos, float Ext)
{
	std::vector<std::shared_ptr<Bodies>> Temp;
	if (!InsidePar(Pos, Ext))
		return Temp;
	//Append all Bods in the Octo and not include current Body
	for (auto& jj : Bods)
	{
			Temp.push_back(jj);
	}
	if (Box0 == NULL)
		return Temp;
	//Get rest of Bodies
	std::vector<std::shared_ptr<Bodies>> Bods0 = Box0->GetQueries(Pos, Ext);
	Temp.insert(Temp.end(), Bods0.begin(), Bods0.end());

	std::vector<std::shared_ptr<Bodies>> Bods1 = Box1->GetQueries(Pos, Ext);
	Temp.insert(Temp.end(), Bods1.begin(), Bods1.end());

	std::vector<std::shared_ptr<Bodies>> Bods2 = Box2->GetQueries(Pos, Ext);
	Temp.insert(Temp.end(), Bods2.begin(), Bods2.end());

	std::vector<std::shared_ptr<Bodies>> Bods3 = Box3->GetQueries(Pos, Ext);
	Temp.insert(Temp.end(), Bods3.begin(), Bods3.end());

	std::vector<std::shared_ptr<Bodies>> Bods4 = Box4->GetQueries(Pos, Ext);
	Temp.insert(Temp.end(), Bods4.begin(), Bods4.end());

	std::vector<std::shared_ptr<Bodies>> Bods5 = Box5->GetQueries(Pos, Ext);
	Temp.insert(Temp.end(), Bods5.begin(), Bods5.end());

	std::vector<std::shared_ptr<Bodies>> Bods6 = Box6->GetQueries(Pos, Ext);
	Temp.insert(Temp.end(), Bods6.begin(), Bods6.end());

	std::vector<std::shared_ptr<Bodies>> Bods7 = Box7->GetQueries(Pos, Ext);
	Temp.insert(Temp.end(), Bods7.begin(), Bods7.end());

	return Temp;
}

std::string CoatlPhysicsEngine::OctoTree::GetAlgorType()
{
	return "OctoTree";
}

void CoatlPhysicsEngine::OctoTree::Erase()
{
	this->Bods.clear();
}
