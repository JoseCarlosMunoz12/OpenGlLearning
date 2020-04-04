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
namespace CoatlPhysicsEngine
{
	class ColShapes
	{
	protected:
		glm::vec3 Pos;
	public:
		ColShapes(glm::vec3 SetPos);
		virtual ~ColShapes();
		glm::vec3 GetPos();
		virtual void SetPos(glm::vec3 NewPOs);
	};
}