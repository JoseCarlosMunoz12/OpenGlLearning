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
namespace CoatlPhysicsEngine {
	class Bod_Base
	{
	protected:
		glm::vec3 Pos;
		glm::vec3 AccumForce;
		glm::vec3 Vel;
		float LinDamp;
		float InvMass;
	public:
		Bod_Base(glm::vec3 Pos);
		Bod_Base(glm::vec3 InitPos, glm::vec3 InitVel);
		~Bod_Base();
		void SetMass(float Mass);
		void SetVel(glm::vec3 SetVel);
	};
}

