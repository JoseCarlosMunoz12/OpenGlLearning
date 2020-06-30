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
#define EPA_TOLERANCE 0.0001
#define EPA_MAX_NUM_FACES 64
#define EPA_MAX_NUM_LOOSE_EDGES 32
#define EPA_MAX_NUM_ITERATIONS 64
namespace CoatlPhysicsEngine {
	//Evolving ID
	// 0 = Still evolving
	// 1 = No intersection
	// 2 = Intersection
	class GJK_Alg
	{
	private:
		float F3Box(glm::vec3 A, glm::vec3 B, glm::vec3 C);
		glm::vec3 TripleCross(glm::vec3 A, glm::vec3 B);
		//Simplex for GJK and EPA
		glm::vec3 Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir);
		glm::vec3 EPA_Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir);
		bool Simplex_Maker(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
			std::vector<glm::vec3>& Verts, glm::vec3& Dir);
	public:
		GJK_Alg();
		~GJK_Alg();
		bool GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1);
		bool EPA_GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,glm::vec3& DistVect);

	};
}