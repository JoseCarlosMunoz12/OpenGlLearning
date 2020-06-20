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
		void SetRadius(float NewR);
		void SetQuat(glm::quat NewQuat) override;
		glm::vec3 Support(glm::vec3 Dir) override;
		glm::vec3 EPA_Support(glm::vec3 Dir) override;
	};
}
