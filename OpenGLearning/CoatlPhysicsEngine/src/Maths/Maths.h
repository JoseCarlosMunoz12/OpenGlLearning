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

#include "../Shapes/SimpleShapes.h"
using namespace CoatlPhysicsEngine;
namespace MATH {
	glm::vec3 Set();
	//Closest points for Shapes and Segments
	glm::vec3 ClosetPoint_Pln(Plane Pl, glm::vec3 Point);
	glm::vec3 ClosestPoint_Seg(std::vector<glm::vec3> trianlge, glm::vec3 Pos, glm::vec3 Point);
	glm::vec3 ClosestPoint_Pnt(std::vector<glm::vec3> Seg, glm::vec3 Pos, glm::vec3 Point);
	//DistanceValue
	float Distance_Pnt(std::vector<glm::vec3> Seg, glm::vec3 Pos, glm::vec3 Point);
	float Distance_Seg(std::vector<glm::vec3> Seg0, glm::vec3 Pos, std::vector<glm::vec3> Seg1);
	float Distance_Seg(std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1);
	float Distance_Tr_Pnt(std::vector<glm::vec3> Tr,glm::vec3 Pos, glm::vec3 Pnt);
	// Bool SegmentTest
	bool ProjColl(std::vector<glm::vec3> Seg, std::vector<glm::vec3> Sh_Vert0, std::vector<glm::vec3> Sh_Vert1);
} 