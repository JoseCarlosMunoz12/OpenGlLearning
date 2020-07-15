#pragma once
#include <vector>
#include "../Shapes/ColShapes.h"
#include "../Physics/Particle.h"
namespace CoatlPhysicsEngine {
	class BodyParts
	{
	private:
		std::shared_ptr<BodyParts> Parent;
		std::shared_ptr<ColShapes> BodPart;
		std::shared_ptr<Particle> BodParticle;
	public:
		BodyParts();
		~BodyParts();
		glm::mat4 GetParMatrix();
		glm::mat4 GetMatrix();
		void UpdateMatrix();
	};
}