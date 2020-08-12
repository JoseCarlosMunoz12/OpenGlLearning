#pragma once
#include "../BodyParts.h"
#include "../../CollisionResolution/Manifoldsh.h"
namespace CoatlPhysicsEngine
{
	class ColJoints
	{
	protected:
		std::weak_ptr<BodyParts> BodA;
		std::weak_ptr<BodyParts> BodB;
		float Max;
	public:
		ColJoints() {};
		~ColJoints() {};
		virtual void SetBodies(std::weak_ptr<BodyParts> NewBodA, std::weak_ptr<BodyParts> NewBodB,
			float Max, float Min = 1.f, glm::quat Angle = glm::quat(1.f, 0.f, 0.f, 0.f)) { return; };
		virtual std::shared_ptr<Manifold> GetResolution() { return std::shared_ptr<Manifold>(); };
		//Sets parameters
		virtual void SetMin(float NewMin) { return; };
		virtual void SetMax(float NewMax) { return; };
		virtual void SetAngle(glm::quat NewAngle) { return; };
		//Get Parameters
		virtual float GetMax() { return this->Max; };
		virtual float GetMin() { return 0.f; };
		virtual glm::quat GetAngle() { return glm::quat(1.f, 0.f, 0.f, 0.f); };
	};
}