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
	//Evolving ID
	// 0 = Still evolving
	// 1 = No intersection
	// 2 = Intersection
	float F3Box(glm::vec3 A, glm::vec3 B, glm::vec3 C);
	glm::vec3 TripleCross(glm::vec3 A, glm::vec3 B, glm::vec3 C);
	bool AddVertex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir, std::vector<glm::vec3> &Vertex);
	int EvolveSimplex(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
		std::vector<glm::vec3> &Vertex, glm::vec3& Dir);
	bool GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1);
}