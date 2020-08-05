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
		glm::quat AxisAngle;
		glm::vec3 RotVel;
		glm::mat3 InvInertia;
		float LinDamp;
		float RotDamp;
		float InvMass;
		virtual void UpdateVel(float dt) { return; };
		//Sleep variables
		bool IsAwake = true;
		bool CanSleep = true;
		float Motion;
	public:
		Bod_Base(glm::vec3 Pos);
		Bod_Base(glm::vec3 InitPos, glm::vec3 InitVel);
		Bod_Base(glm::vec3 InitPos, glm::vec3 InitVel, float InitDamp);
		~Bod_Base();
		void SetMass(float Mass);
		void SetVel(glm::vec3 SetVel);
		void AddVel(glm::vec3 DelVel);
		void SetId(int NewId);
		void SetPos(glm::vec3 NewPos);		
		virtual void SetQuat(glm::quat NewQuat) { return; };
		glm::quat GetQuat() { return this->AxisAngle; };
		glm::vec3 GetPos();
		glm::vec3 GetVel();
		int GetPhyID() { return this->PhysicsID; };
		float GetMass() { return 1 / this->InvMass; };
		//Virtual Functions for the Particle and Rigid Bodies
		virtual glm::vec3 UpdatePos(float dt) { return glm::vec3(0.f); };
		virtual void ResetForce() { return; };
		virtual void AcumForce(glm::vec3 Force) { return; };
		//Rigid Bodies functions
		virtual void AddForceAtBodyPoint(glm::vec3 Force, glm::vec3 Pnt) { return; };
		virtual void AcumTorque(glm::vec3 Torque) { return; };
		virtual glm::vec3 GetRotVel() { return glm::vec3(0.f); };
		glm::mat3 GetInertia() { return glm::inverse(this->InvInertia);};
		virtual void SetInertia(glm::mat3 Inertia) { return; };
		virtual void SetRotVel(glm::vec3 NewRotVel) { return; };
		virtual void AddRotVel(glm::vec3 DelRotVel) { return; };
		//Sleep Functions
		virtual void SetAwake(bool Awake) { return; };
		virtual bool GetAwakeStatus() { return true; }; 
		virtual void SetCanSleep(bool Can) { return; };
		virtual bool SleepStatus() { return true; };
	};
}

