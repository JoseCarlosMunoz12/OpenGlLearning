#pragma once

#include "Vertex.h"

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <vector>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
class Collision
{
private:
	glm::vec3 Pos;
public:
	Collision()
	{

	}
	~Collision()
	{

	}
	virtual void Update()
	{

	}

};