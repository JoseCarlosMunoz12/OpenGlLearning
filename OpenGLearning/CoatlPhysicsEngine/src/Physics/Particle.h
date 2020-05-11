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
	class Particle 
	{
	private:
		glm::vec3 Vel;
		float Damping;
		float InverMass;
		void UpdateVel(glm::vec3 Acc, float dt);
	public:
		Particle(glm::vec3 InitVel,float InitDamp);
		Particle(glm::vec3 InitVel);
		Particle();
		~Particle();
		glm::vec3 UpdatePos(glm::vec3 Pos, glm::vec3 Ac, float dt);
		void SetVel(glm::vec3 NewVel);
		glm::vec3 GetVel();
		float GetInverseMass();
		void ChangeInverseMass(float Mass);
	};
}