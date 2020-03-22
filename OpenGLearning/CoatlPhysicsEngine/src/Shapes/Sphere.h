#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine {
	class Sphere :public ColShapes
	{
		float Radius;
	public:
		Sphere(glm::vec3 SetPos,float SetRadius);
		~Sphere();
		float GetRadius();
	};
}
