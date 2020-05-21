#pragma once
#include "../Bodies/Bodies.h"
	struct Contact
	{
		glm::vec3 ContactPoint;
		glm::vec3 Normal;
		float Penetration;
	};
	struct Manifold
	{
		std::vector<std::shared_ptr<Contact>> Contacts;
		unsigned ContactCount;
		std::shared_ptr<Bodies> Bod0;
		std::shared_ptr<Bodies> Bod1;
	};