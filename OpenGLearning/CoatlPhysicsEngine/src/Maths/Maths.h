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
namespace MATH {
	glm::vec3 Set();
	//Closest points for Shapes and Segments
	glm::vec3 ClosestPoint_Seg(std::vector<glm::vec3> trianlge, glm::vec3 Pos, glm::vec3 Point);
	glm::vec3 ClosestPoint_Pnt(std::vector<glm::vec3> Seg, glm::vec3 Pos, glm::vec3 Point);
	//DistanceValue
	float Distance_Cap_Pnt(std::vector<glm::vec3> Seg, glm::vec3 Pos, glm::vec3 Point);
}