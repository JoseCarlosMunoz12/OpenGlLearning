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
	private:
		glm::vec3 Pos;
	public:		
		ColShapes(glm::vec3 SetPos);
		~ColShapes();
		virtual std::string GetShapeType();
		glm::vec3 GetPos();
	};
}