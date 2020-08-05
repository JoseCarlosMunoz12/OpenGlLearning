#pragma once
#include "Bod_Base.h"
namespace CoatlPhysicsEngine {
	class RigidBodies : public Bod_Base
	{
	private:
		const float Epsilon = 0.00001f;
		glm::mat4 TransformMatrix;
		glm::mat4 InvIntertiaWSpace;
		glm::mat3 InvInertia;
		glm::vec3 TorqueAcum;
		glm::vec3 AngularAccelration;
		void UpdateRot(float dt);
		void UpdateMatrix();
		void CalcDerivedData();
		void TransformInertiaTensor();
		void UpdateVel(float dt) override;
		void AddForceAtPoint(glm::vec3 Force, glm::vec3 Pnt);
	public:
		RigidBodies(glm::vec3 Pos);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp);
		~RigidBodies();
		void SetInertia(glm::mat3 InitInertia);
		void AcumTorque(glm::vec3 Torque) override;
		void AddForceAtBodyPoint(glm::vec3 Force, glm::vec3 Pnt) override;
		void SetQuat(glm::quat NewQuat) override;
		void AcumForce(glm::vec3 Force) override;
		void ResetForce() override;
		//Rotational functions
		glm::vec3 GetRotVel() override;
		void SetRotVel(glm::vec3 NewRotVel) override;
		void AddRotVel(glm::vec3 DelRotVel) override;
		//Main function to update physics
		glm::vec3 UpdatePos(float dt) override;
		//sleep functions
		bool GetAwakeStatus() override;
		void SetCanSleep(bool Can) override;
		void SetAwake(bool Awake) override;
	};	
}
