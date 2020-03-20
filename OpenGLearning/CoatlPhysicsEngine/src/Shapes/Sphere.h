#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine {
	class Sphere :public ColShapes
	{
		float Radius;
	public:
		Sphere(float SetRadius);
		~Sphere();
		float GetRadius();
		std::string GetShapeType() override;
	};
}
