#pragma once
#include "Bod_Base.h"
namespace CoatlPhysicsEngine {
	class RigidBodies : public Bod_Base
	{
	private:
		const float Epsilon = 0.00001f;
		glm::mat4 TransformMatrix;
		glm::mat3 InvInertia;
		glm::vec3 RotVel;
		glm::vec3 TorqueAcum;
		glm::vec3 AngularAccelration;
		glm::quat AxisAngle;
		float RotDamp;
		//Sleep variables
		bool IsAwake = true;
		bool CanSleep = true;
		float Motion;
		void UpdateRot(float dt);
		void UpdateMatrix();
		void CalcDerivedData();
		void TransformInertiaTensor();
	public:
		RigidBodies(glm::vec3 Pos);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp);
		~RigidBodies();
		void SetInertia(glm::mat3 InitInertia);
		void AddTorque(glm::vec3 Torque);
		void SetAwake(bool Awake);
		void AcumForce(glm::vec3 Force) override;
		void ResetForce() override;
		glm::vec3 UpdatePos(float dt) override;
		bool GetAwakeStatus();
		void SetCanSleep(bool Can);
	};	
}
