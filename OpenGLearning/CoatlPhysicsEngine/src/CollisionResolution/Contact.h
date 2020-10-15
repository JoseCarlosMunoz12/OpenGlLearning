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
	class Contact
	{
	public:
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
		inline bool operator< (const Contact& B)
		{
			int IDA = Bods[0]->GetID();
			int IDB = B.Bods[0]->GetID();
			if (IDA == IDB)
				return dt0 < B.dt0;
			return IDA < IDB;
		}
	};
}
