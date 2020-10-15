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
#include "../Bodies/Bod_Collection.h"
#include "Contact.h"
namespace CoatlPhysicsEngine {
	
	class Col_Resolution
	{
	private:
		//Iterations Count
		int VelIt;
		int Posit;
		int Took = 0;
		int VelTook = 0;
		// Impulse resolition
	public:
		int GetTook() { return this->Took; };
		int GetVelTook() { return this->VelTook; };
		Col_Resolution(int InitVel, int InitPos);
		~Col_Resolution();
		//Resolve Manifolds
		 void ResolveContacts(std::vector<std::shared_ptr<Contact>> Contact, float dt);
	};
}