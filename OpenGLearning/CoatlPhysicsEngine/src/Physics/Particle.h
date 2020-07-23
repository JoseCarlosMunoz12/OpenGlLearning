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
	protected:
		//Linear Information
		float Damping;
		glm::vec3 Pos;
		glm::vec3 Vel;
		glm::quat AxisAngle;
		//Other General information
		float InverMass;
		glm::vec3 Acum_Force;
		int PhysicsID;
		void UpdateVel(float dt);
	public:
		Particle(glm::vec3 Pos,glm::vec3 InitVel,float InitDamp);
		Particle(glm::vec3 Pos,glm::vec3 InitVel);
		Particle(glm::vec3 Pos);
		~Particle();
		virtual glm::vec3 UpdatePos(float dt);
		void SetVel(glm::vec3 NewVel);
		glm::vec3 GetVel();
		glm::vec3 GetPos();
		glm::quat GetQuat();
		float GetMass();
		void ChangeInverseMass(float Mass);
		virtual void ResetForce();
		virtual void AcumForce(glm::vec3 Force);
		void SetMass(float Mass);
		void SetPos(glm::vec3 Pos);
		void SetQuat(glm::quat NewQuat);
	};
}