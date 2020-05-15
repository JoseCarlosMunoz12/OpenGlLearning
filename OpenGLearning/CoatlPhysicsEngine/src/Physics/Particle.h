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
		glm::vec3 Acum_Force;
		void UpdateVel(float dt);
	public:
		Particle(glm::vec3 InitVel,float InitDamp);
		Particle(glm::vec3 InitVel);
		Particle();
		~Particle();
		glm::vec3 UpdatePos(glm::vec3 Pos, float dt);
		void SetVel(glm::vec3 NewVel);
		glm::vec3 GetVel();
		float GetInverseMass();
		void ChangeInverseMass(float Mass);
		void ResetForce();
		void AcumForce(glm::vec3 Force);
		void SetMass(float Mass);
	};
}