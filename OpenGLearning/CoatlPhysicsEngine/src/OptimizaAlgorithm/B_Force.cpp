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
	return Bods;
}

std::string B_Force::GetAlgorType()
{
	return "Brute Force";
}
void B_Force::Erase()
{
	this->Bods.clear();
}