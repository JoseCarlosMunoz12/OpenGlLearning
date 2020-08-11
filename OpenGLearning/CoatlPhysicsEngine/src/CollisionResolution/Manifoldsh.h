#pragma once
#include "../Bodies/Bodies.h"
	struct Contact
	{
		glm::vec3 ContactPoint;
		glm::vec3 Normal;
		float Penetration;
		float Friction;
		float Restituion;
	};
	struct Manifold
	{
		std::vector<std::shared_ptr<Contact>> Contacts;
		unsigned ContactCount;
		std::shared_ptr<Bodies> Bod0;
		std::shared_ptr<Bodies> Bod1;
		//-1 for both are movable
		//0 For Bod0 is only Movable
		//1 For Bod1 is only Movable
		int ID;
	};