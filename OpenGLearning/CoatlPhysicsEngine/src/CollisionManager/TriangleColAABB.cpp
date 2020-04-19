#include "TriangleColAABB.h"
using namespace CoatlPhysicsEngine;

bool TriangleColAABB::TrColAABB(Triangles Tr, AABB_Obj AABB)
{
	float P0, P1, P2, R;

	glm::vec3 C = AABB.GetPos();
	glm::vec3 Ex = AABB.GetLengths();
	glm::vec3 TrPos = Tr.GetPos();
	std::vector<glm::vec3> TrPnt = Tr.GetSegments();

	glm::vec3 V0 = (TrPnt[0] + TrPos) - C;
	glm::vec3 V1 = (TrPnt[1] + TrPos) - C;
	glm::vec3 V2 = (TrPnt[2] + TrPos) - C;
	
	glm::vec3 F0 = V1 - V0;
	glm::vec3 F1 = V2 - V1;
	glm::vec3 F2 = V0 - V2;
	//Region test in this scope
	{
		//Tests Region a00
		glm::vec3 a00(0.f, -F0.z, F0.y);
		P0 = glm::dot(V0, a00);
		P2 = glm::dot(V1, a00);
		P1 = glm::dot(V2, a00);
		R = Ex.y * glm::abs(F0.z) + Ex.z * glm::abs(F0.y);
		if (glm::max(-std::max(P0, P1, P2), std::min(P0, P1, P2)) > R)
		{
			return false;
		}
		//Tests Region a01
		glm::vec3 a01(0.f, -F1.z, F1.y);
		P0 = glm::dot(V0, a01);
		P2 = glm::dot(V1, a01);
		P1 = glm::dot(V2, a01);
		R = Ex.y * glm::abs(F1.z) + Ex.z * glm::abs(F1.y);
		if (glm::max(-std::max(P0, P1, P2), std::min(P0, P1, P2)) > R)
		{
			return false;
		}
		//Tests Region a02
		glm::vec3 a02(0.f, -F2.z, F2.y);
		P0 = glm::dot(V0, a02);
		P2 = glm::dot(V1, a02);
		P1 = glm::dot(V2, a02);
		R = Ex.y * glm::abs(F2.z) + Ex.z * glm::abs(F2.y);
		if (glm::max(-std::max(P0, P1, P2), std::min(P0, P1, P2)) > R)
		{
			return false;
		}
		//Tests Region a10
		glm::vec3 a10(F0.z, 0.f, -F0.x);
		P0 = glm::dot(V0, a10);
		P2 = glm::dot(V1, a10);
		P1 = glm::dot(V2, a10);
		R = Ex.y * glm::abs(F0.z) + Ex.z * glm::abs(F0.y);
		if (glm::max(-std::max(P0, P1, P2), std::min(P0, P1, P2)) > R)
		{
			return false;
		}

		//Tests Region a11
		glm::vec3 a11(F1.z, 0.f, -F1.x);
		P0 = glm::dot(V0, a11);
		P2 = glm::dot(V1, a11);
		P1 = glm::dot(V2, a11);
		R = Ex.y * glm::abs(F1.z) + Ex.z * glm::abs(F1.y);
		if (glm::max(-std::max(P0, P1, P2), std::min(P0, P1, P2)) > R)
		{
			return false;
		}
		//Tests Region a12
		glm::vec3 a12(F2.z, 0.f, -F2.x);
		P0 = glm::dot(V0, a12);
		P2 = glm::dot(V1, a12);
		P1 = glm::dot(V2, a12);
		R = Ex.y * glm::abs(F2.z) + Ex.z * glm::abs(F2.y);
		if (glm::max(-std::max(P0, P1, P2), std::min(P0, P1, P2)) > R)
		{
			return false;
		}

		//Tests Region a20
		glm::vec3 a20(-F0.y, -F0.x, 0.f);
		P0 = glm::dot(V0, a20);
		P2 = glm::dot(V1, a20);
		P1 = glm::dot(V2, a20);
		R = Ex.y * glm::abs(F0.z) + Ex.z * glm::abs(F0.y);
		if (glm::max(-std::max(P0, P1, P2), std::min(P0, P1, P2)) > R)
		{
			return false;
		}
		//Tests Region a21
		glm::vec3 a21(-F1.y, -F1.x, 0.f);
		P0 = glm::dot(V0, a21);
		P2 = glm::dot(V1, a21);
		P1 = glm::dot(V2, a21);
		R = Ex.y * glm::abs(F1.z) + Ex.z * glm::abs(F1.y);
		if (glm::max(-std::max(P0, P1, P2), std::min(P0, P1, P2)) > R)
		{
			return false;
		}
		//Tests Region a12
		glm::vec3 a22(-F2.y, -F2.x, 0.f);
		P0 = glm::dot(V0, a22);
		P2 = glm::dot(V1, a22);
		P1 = glm::dot(V2, a22);
		R = Ex.y * glm::abs(F2.z) + Ex.z * glm::abs(F2.y);
		if (glm::max(-std::max(P0, P1, P2), std::min(P0, P1, P2)) > R)
		{
			return false;
		}
	}
	//Region test the three axes for the face normals of AABB
	{
		if (std::max(V0.x,V1.x,V2.x) < -Ex.x ||
			std::min(V0.x, V1.x, V2.x) > Ex.x)
		{
			return false;
		}
		if (std::max(V0.y, V1.y, V2.y) < -Ex.y ||
			std::min(V0.y, V1.y, V2.y) > Ex.y)
		{
			return false;
		}
		if (std::max(V0.z, V1.z, V2.z) < -Ex.z ||
			std::min(V0.z, V1.z, V2.z) > Ex.z)
		{
			return false;
		}
	}
	//Ratio test seperatin axis
	Plane Pl;
	Pl.Normal = glm::cross(F0, F1);
	Pl.D = glm::dot(Pl.Normal, V0);
	return MATH::TestAABBPlane(AABB, Pl);
}

TriangleColAABB::TriangleColAABB()
{
}

TriangleColAABB::~TriangleColAABB()
{
}
