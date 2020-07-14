#pragma once
#include <vector>
#include "../Shapes/ColShapes.h"
#include "../Physics/Particle.h"
namespace CoatlPhysicsEngine {
	class BodyParts
	{
	private:
		std::shared_ptr<BodyParts> Parent;
		glm::mat4 RelMat;
		glm::vec3 Pos;
		float Angle;
		glm::vec3 UnitVec;
		std::shared_ptr<ColShapes> BodPart;
		std::shared_ptr<Particle> BodParticle;
	public:
		BodyParts();
		~BodyParts();
	};
}