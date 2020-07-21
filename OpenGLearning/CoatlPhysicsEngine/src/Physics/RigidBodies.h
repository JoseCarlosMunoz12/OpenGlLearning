#pragma once
#include "Particle.h"
namespace CoatlPhysicsEngine {
	class RigidBodies : public Particle
	{
	private:
		glm::mat4 TransformMatrix;
		glm::mat3 InvIntertia;
		glm::vec3 RotVel;
		glm::vec3 TorqueAcum;
		float RotDamp;
		void UpdateRot(float dt);
		void UpdateMatrix();
		void CalcDerivedData();
	public:
		RigidBodies(glm::vec3 Pos);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel);
		RigidBodies(glm::vec3 Pos, glm::vec3 InitVel, float InitDamp);
		void SetInertia(glm::mat3 InitInitertia);

		void AddTorque(glm::vec3 Torque);
		glm::vec3 UpdatePos(float dt) override;
		~RigidBodies();
	};	
}
