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
namespace CoatlPhysicsEngine {
	class Contacts {
	private:
	public:
		Contacts();
		~Contacts();
		std::shared_ptr<Bodies> Bods[2];
		std::vector<glm::vec3> ContactPoint;
		//relative contact Locations
		std::vector<glm::vec3> R0;
		std::vector<glm::vec3> R1;
		float dt0;//Time of Contact
		float dt1;//TIme left in frame
		glm::vec3 Normal;
		float Penetration;
		float Friction;
		float Restituion;
	};
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
		 void ResolveContacts(std::vector<std::shared_ptr<Contacts>> Contact, float dt);
	};
}