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
#define GJK_MAX_ITERATIONS 20
namespace CoatlPhysicsEngine {
	struct gjk_support
	{
		int AId, BId;
		glm::vec3 A;
		glm::vec3 B;
		glm::vec3 D;
		glm::vec3 DA;
		glm::vec3 DB;
	};
	struct gjk_simplex
	{
		int Max_iter, iter;
		struct gjk_vertex
		{
			glm::vec3 A;
			glm::vec3 B;
			glm::vec3 P;
			int AId, BId;
		}V[4];
		glm::vec3 BC;
		float D;
	};
	struct gjk_result
	{
		bool Hit;
		glm::vec3 P0;
		glm::vec3 P1;
		float Dis;
		int Iter;
	};
	bool GJK(std::shared_ptr<gjk_simplex> &S, std::shared_ptr<gjk_simplex> &Sup);
	void GJK_analyze(std::shared_ptr<gjk_result> &Res);
	void GJK_quat(std::shared_ptr<gjk_result>& Res, float A_radius, float B_Radius);
}