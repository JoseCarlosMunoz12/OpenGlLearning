#include "TriangleColSphere.h"
using namespace CoatlPhysicsEngine;
bool TriangleColSphere::TrColSphere(Triangles Tr, Sphere Sph)
{
	glm::vec3 Pos = Sph.GetPos();
	float Dis = MATH::Distance_Tr_Pnt(Tr.GetSegments(),Tr.GetPos(),Pos);
	float Rad = Sph.GetRadius();	
	return Rad >= Dis; 
}

TriangleColSphere::TriangleColSphere()
{

}

TriangleColSphere::~TriangleColSphere()
{

}
