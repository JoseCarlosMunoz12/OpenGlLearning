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
	static enum SHAPE_TYPE
	{
		S_BASE,
		S_SPHERE,
		S_ABOX,
		S_OBOX,
		S_CYLINDER,
		S_CONVEX
	};
	class ColShapes
	{
	private:
		glm::vec3 Pos;
	public:		
		virtual SHAPE_TYPE GetType();
		ColShapes(glm::vec3 SetPos);
		~ColShapes();
		glm::vec3 GetPos();
	};
}