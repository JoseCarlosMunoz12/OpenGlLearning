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
	public:
		ColJoints() {};
		~ColJoints() {};
		virtual void SetBodies(std::weak_ptr<BodyParts> NewBodA, std::weak_ptr<BodyParts> NewBodB,
			float Max, float Min = 1.f, glm::quat Angle = glm::quat(1.f, 0.f, 0.f, 0.f)){ return; };
		virtual std::shared_ptr<Manifold> GetResolutin() { return std::shared_ptr<Manifold>(); };
	};
}