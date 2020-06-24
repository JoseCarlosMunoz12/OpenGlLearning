#pragma once
#include <string>
#include <iostream>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
#include <vector>
#include "../Shapes/ColShapes.h"
namespace CoatlPhysicsEngine {
	class SAT
	{
	private:
		float ProjPen(glm::vec3& Normal, std::vector<glm::vec3> Sh_Vert0, std::vector<glm::vec3> Sh_Vert1);
		float SAT_Algo(std::vector<glm::vec3> Norm0, std::vector<glm::vec3> Norm1,
			std::vector<glm::vec3> Pnts0, std::vector<glm::vec3> Pnts1,
			glm::vec3& Norm);
	public:
		SAT();
		~SAT();
		glm::vec3 GetPenetrationContacts(std::shared_ptr<ColShapes> Bod0, std::shared_ptr<ColShapes> Bod1);
	};
}