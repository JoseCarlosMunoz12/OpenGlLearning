#pragma once
#include "Bod_Base.h"
namespace CoatlPhysicsEngine {
	class Particle :public Bod_Base
	{
	protected:
		void UpdateVel(float dt) override;
	public:
		Particle(glm::vec3 Pos,glm::vec3 InitVel,float InitDamp);
		Particle(glm::vec3 Pos,glm::vec3 InitVel);
		Particle(glm::vec3 Pos);
		~Particle();
		glm::vec3 UpdatePos(float dt) override;
		float GetMass();
		void ChangeInverseMass(float Mass);
		void ResetForce() override;
		void AcumForce(glm::vec3 Force) override;
		void SetMass(float Mass);
	};
}