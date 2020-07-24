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
		int PhysicsID;
		glm::vec3 Pos;
		glm::vec3 AccumForce;
		glm::vec3 Vel;
		float LinDamp;
		float InvMass;
		virtual void UpdateVel(float dt) { return; };
	public:
		Bod_Base(glm::vec3 Pos);
		Bod_Base(glm::vec3 InitPos, glm::vec3 InitVel);
		Bod_Base(glm::vec3 InitPos, glm::vec3 InitVel, float InitDamp);
		~Bod_Base();
		void SetMass(float Mass);
		void SetVel(glm::vec3 SetVel);
		void SetId(int NewId);
		void SetPos(glm::vec3 NewPos);		
		virtual void SetQuat(glm::quat NewQuat) { return; };
		glm::vec3 GetPos();
		glm::vec3 GetVel();
		float GetMass() { return 1 / this->InvMass; };
		//Virtual Functions for the Particle and Rigid Bodies
		virtual glm::vec3 UpdatePos(float dt) { return glm::vec3(0.f); };
		virtual void ResetForce() { return; };
		virtual void AcumForce(glm::vec3 Force) { return; };
		virtual void AcumTorque(glm::vec3 Torque) { return; };
	};
}

