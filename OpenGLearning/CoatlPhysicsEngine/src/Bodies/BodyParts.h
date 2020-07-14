#pragma once
#include <vector>
#include "../Shapes/ColShapes.h"
#include "../Physics/Particle.h"
namespace CoatlPhysicsEngine {
	class BodyParts
	{
	private:

	public:
		std::shared_ptr<ColShapes> BodPart;
		std::shared_ptr<Particle> BodParticle;
	};
}