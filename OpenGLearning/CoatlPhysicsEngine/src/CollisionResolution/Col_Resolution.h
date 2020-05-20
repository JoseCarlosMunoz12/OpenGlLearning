#pragma once
#include <string>
#include <iostream>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
#include <vector>
#include "../Bodies/Bodies.h"
namespace CoatlPhysicsEngine {
	struct Contact
	{
		glm::vec3 ContactPoint;
		glm::vec3 Normal;
		float Penetration;
	};
	struct Manifold
	{
		std::vector<std::shared_ptr<Contact>> Contacts;
		unsigned ContactCountl;
		std::shared_ptr<Bodies> Bod0;
		std::shared_ptr<Bodies> Bod1;
	};
	class Col_Resolution
	{
	};
}