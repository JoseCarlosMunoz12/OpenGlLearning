#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine {
	struct Plane
	{
		glm::vec3 Normal;
		float D;
	};
	struct C_Plane
	{
		glm::vec3 Normal;
		float Bias;
		C_Plane(std::vector<glm::vec3> TR)
		{
			Normal = glm::cross((TR[1]-TR[0]), (TR[2] - TR[1]));
			Bias = glm::dot(Normal, TR[1]);
		}
		bool SameLocPl(std::vector<glm::vec3> Pnt)
		{

		}
	};
}