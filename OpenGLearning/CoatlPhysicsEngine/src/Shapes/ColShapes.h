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

#include "../Maths/Maths.h"

namespace CoatlPhysicsEngine
{
	class ColShapes
	{
	protected:
		glm::vec3 Pos;
		glm::quat QuatAngle;
	public:
		ColShapes(glm::vec3 SetPos);
		virtual ~ColShapes();
		glm::vec3 GetPos();
		glm::quat GetQuatAngle();
		virtual void SetPos(glm::vec3 NewPOs);
		virtual void SetQuat(glm::quat NewQuat) { return; };
		virtual glm::vec3 Support(glm::vec3 Dir);
		virtual glm::vec3 EPA_Support(glm::vec3 Dir);
		//
		virtual std::vector<glm::vec3> GetVertices();
		virtual std::vector<glm::vec3> GetNormals();
	};
}