#include "B_Force_Self.h"
using namespace CoatlPhysicsEngine;

B_Force_Self::B_Force_Self()
{
}

B_Force_Self::~B_Force_Self()
{
}

bool B_Force_Self::Insert(std::shared_ptr<Bodies> Bod)
{
	this->Bods.push_back(Bod);
	return true;
}

std::vector<std::shared_ptr<Bodies>> B_Force_Self::GetQueries(std::shared_ptr<Bodies> Bod, float Ext)
{
	return this->Bods;
}

std::string B_Force_Self::GetAlgorType()
{
	return "BruteForceSelf";
}
void B_Force_Self::Erase()
{
	this->Bods.clear();
}
