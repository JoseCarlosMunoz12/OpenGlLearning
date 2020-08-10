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
	class GJK_Alg
	{
	private:
		//Other Vectors functions
		float F3Box(glm::vec3 A, glm::vec3 B, glm::vec3 C);
		glm::vec3 TripleCross(glm::vec3 A, glm::vec3 B);
		int Tr_Farthest_Point(std::vector<glm::vec3> Vec);
		//regular Support Function	
		glm::vec3 Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir);
		//sweep test function
		glm::vec3 Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
			std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1 ,glm::vec3 Dir);
		glm::vec3 Support(std::vector<glm::vec3> Seg, glm::vec3 Dir);
		bool Simplex_Maker(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Pos0, glm::vec3 Pos1,
			std::vector<glm::vec3>& Verts, glm::vec3& Dir);
		//EPA GJK functions
		glm::vec3 EPA_Support(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, glm::vec3 Dir);
		glm::vec3 EPA(std::vector<glm::vec3> Verts,
			std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1, float& Pen);
		glm::vec3 ClosestPoint(std::vector<glm::vec3> Verts);
		std::vector<glm::vec3> NewSimplex(std::vector<glm::vec3> Verts, glm::vec3 NewPoint);
		float Cl_Dist(std::vector<glm::vec3> Verts);
		glm::vec3 C_F_E(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1);
		//Simplex for GJK
		bool Simplex_Maker(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
			std::vector<glm::vec3>& Verts, glm::vec3& Dir);
		std::vector<int> C_F_S(std::vector<glm::vec3>& Verts);
	public:
		GJK_Alg();
		~GJK_Alg();
		bool GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1);
		bool GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,
			std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1);
		bool EPA_GJK(std::shared_ptr<ColShapes> Shape0, std::shared_ptr<ColShapes> Shape1,glm::vec3& DistVect,float& Pen);

	};
}