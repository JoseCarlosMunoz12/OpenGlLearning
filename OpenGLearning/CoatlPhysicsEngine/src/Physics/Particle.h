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
		glm::vec3 Pos;
		glm::vec3 Vel;
		float Damping;
		float InverMass;
		glm::vec3 Acum_Force;
		void UpdateVel(float dt);
		int PhysicsID;
		glm::quat AxisAngle;
	public:
		Particle(glm::vec3 Pos,glm::vec3 InitVel,float InitDamp);
		Particle(glm::vec3 Pos,glm::vec3 InitVel);
		Particle(glm::vec3 Pos);
		~Particle();
		glm::vec3 UpdatePos(float dt);
		void SetVel(glm::vec3 NewVel);
		glm::vec3 GetVel();
		glm::vec3 GetPos();
		glm::quat GetQuat();
		float GetMass();
		void ChangeInverseMass(float Mass);
		void ResetForce();
		void AcumForce(glm::vec3 Force);
		void SetMass(float Mass);
		void SetPos(glm::vec3 Pos);
		void SetQuat(glm::quat NewQuat);
	};
}