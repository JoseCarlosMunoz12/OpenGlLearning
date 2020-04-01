#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine{
	class Capsule: public ColShapes
	{
		float Radius;
		float Length;
	public:
		Capsule(glm::vec3 Pos,float InitRadius,float InitLength);
		~Capsule();
	};
}