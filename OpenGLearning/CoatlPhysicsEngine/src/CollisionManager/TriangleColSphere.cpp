#include "TriangleColSphere.h"
using namespace CoatlPhysicsEngine;
bool TriangleColSphere::TrColSphere(Triangles Tr, Sphere Sph)
{
	glm::vec3 Pos = Sph.GetPos();
	glm::vec3 Loc = Tr.GetClosestPoint(Sph.GetPos());
	float Rad = Sph.GetRadius();
	Rad = Rad * Rad;
	float Dis = glm::distance(Loc, Pos);
	float DisSq = glm::dot((Loc - Pos), (Loc - Pos));
	return Rad >= DisSq; 
}

TriangleColSphere::TriangleColSphere()
{

}

TriangleColSphere::~TriangleColSphere()
{

}
