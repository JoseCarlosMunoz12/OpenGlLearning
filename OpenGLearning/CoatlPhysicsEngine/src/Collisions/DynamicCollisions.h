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

#include "Shapes/ColShapes.h"

namespace CoatlPhysicsEngine {
	class DynamicCollisions
	{
	private:
		std::vector<ColShapes*> ColShapes;
	public:
		DynamicCollisions();
		~DynamicCollisions();
	};
}
