#include "TriangleColSphere.h"
using namespace CoatlPhysicsEngine;
bool TriangleColSphere::TrColSphere(Triangles Tr, Sphere Sph)
{
	glm::vec3 Pos = Sph.GetPos();
	glm::vec3 Loc = Tr.GetClosestPoint(Pos);
	float Rad = Sph.GetRadius();
	float Dis = glm::distance(Loc, Pos);
	return Rad >= Dis; 
}

TriangleColSphere::TriangleColSphere()
{

}

TriangleColSphere::~TriangleColSphere()
{

}
