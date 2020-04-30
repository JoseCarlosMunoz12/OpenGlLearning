#include "B_Force.h"
using namespace CoatlPhysicsEngine;
B_Force::B_Force()
{
}

B_Force::~B_Force()
{
}

bool B_Force::Insert(std::shared_ptr<Bodies> Bod)
{
	this->Bods.push_back(Bod);
	return true;
}

std::vector<std::shared_ptr<Bodies>> B_Force::GetQueries(std::shared_ptr<Bodies> Bod, float Ext)
{
	int Count = 0;
	for (auto& JJ : Bods)
	{
		if (JJ->GetID() == Bod->GetID())
		{
			break;
			Count++;
		}
		Count++;
	}
	
	std::vector<std::shared_ptr<Bodies>> SetBods;
	SetBods.insert(SetBods.end(), Bods.begin() + Count, Bods.end());
	return SetBods;
}

std::string B_Force::GetAlgorType()
{
	return "Brute Force";
}
