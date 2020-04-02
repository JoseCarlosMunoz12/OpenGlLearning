#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine{
	class Capsule: public ColShapes
	{
		float Radius;
		glm::vec3 APos;
		glm::vec3 BPos;
	public:
		Capsule(glm::vec3 Pos,float InitRadius,float InitLength);
		~Capsule();
		float GetRadius();
		glm::vec3 ClosestPoint(glm::vec3 Point);
	};
}