#pragma once
#include <iostream>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <string>
#include <vector>
#include <SOIL2.h>


class MipMap
{
private:
	std::vector<std::vector<glm::vec3>> MipMapHolder;
	MipMap()
	{

	}

	~MipMap()
	{

	}
	glm::vec3 ReturnValue(int Xpos, int YPos)
	{

	}
};
