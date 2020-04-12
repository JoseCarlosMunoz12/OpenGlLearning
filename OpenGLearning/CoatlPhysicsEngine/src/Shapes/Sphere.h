#pragma once
#include "ColShapes.h"
#include "../Maths/Maths.h"
namespace CoatlPhysicsEngine {
	class Sphere :public ColShapes
	{
		float Radius;
	public:
		Sphere(glm::vec3 SetPos,float SetRadius);
		~Sphere();
		float GetRadius();
		void SetRadius(float NewR);
	};
}
