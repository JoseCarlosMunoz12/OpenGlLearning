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
	glm::vec3 ClosestPoint_Seg(std::vector<glm::vec3> Seg, glm::vec3 Point );
	glm::vec3 ClosestPoint_Pnt(std::vector<glm::vec3> Seg, glm::vec3 Pos, glm::vec3 Point);
	void ClosestSeg_Seg(std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1,
		glm::vec3 &Pos0,glm::vec3 &Pos1);
	//DistanceValue
	float Distance_Pnt(std::vector<glm::vec3> Seg, glm::vec3 Pos, glm::vec3 Point);
	float Distance_Seg(std::vector<glm::vec3> Seg0, glm::vec3 Pos, std::vector<glm::vec3> Seg1);
	float Distance_Seg(std::vector<glm::vec3> Seg0, std::vector<glm::vec3> Seg1);
	float Distance_Tr_Pnt(std::vector<glm::vec3> Tr,glm::vec3 Pos, glm::vec3 Pnt);
	// Bool SegmentTest
	bool ProjColl(std::vector<glm::vec3> Seg, std::vector<glm::vec3> Sh_Vert0, std::vector<glm::vec3> Sh_Vert1);
	bool ProjColl(glm::vec3 Normal, std::vector<glm::vec3> Sh_Vert0, std::vector<glm::vec3> Sh_Vert1);
	bool SATColCheck(std::vector<glm::vec3> Norm0, std::vector<glm::vec3> Norm1,
		std::vector<glm::vec3> Pnts0,std::vector<glm::vec3> Pnts1);
	//Return Changes
	void SetMaxMins(glm::vec3 &Max, glm::vec3 &Min,glm::vec3 NewVal);
	// SAT Functions
	float ProjPen(glm::vec3 &Normal, std::vector<glm::vec3> Sh_Vert0, std::vector<glm::vec3> Sh_Vert1);
	float SATContact(std::vector<glm::vec3> Norm0, std::vector<glm::vec3> Norm1,
	std::vector<glm::vec3> Pnts0, std::vector<glm::vec3> Pnts1,glm::vec3 &Norm);
	//GJK Functions
	glm::vec3 MaxDot(std::vector<glm::vec3> Pnts, glm::vec3 Dir);
	glm::vec3 Support(std::vector<glm::vec3> Pnt0, std::vector<glm::vec3> Pnt1, glm::vec3 Dir);
	bool DoSimplex(std::vector<glm::vec3> &A, glm::vec3 &Dir);
	bool GJK_Algorithm(std::vector<glm::vec3> Pnt0, std::vector<glm::vec3> Pnt1);
}