#include "AABB_Obj.h"
using namespace CoatlPhysicsEngine;

float AABB_Obj::GetPoint(float P, float Max, float Min)
{
    if (P < Min)
        P = Min;
    if (P > Max)
        P = Max;
    return P;
}

AABB_Obj::AABB_Obj(glm::vec3 SetPos,
    float DimXYZ)
    :ColShapes(SetPos)
{
    this->XLength = DimXYZ;
    this->YLength = DimXYZ;
    this->ZLength = DimXYZ;
}

AABB_Obj::AABB_Obj(glm::vec3 SetPos,
    float DimX, float DimYZ)
    : ColShapes(SetPos)
{
    this->XLength = DimX;
    this->YLength = DimYZ;
    this->ZLength = DimYZ;
}

AABB_Obj::AABB_Obj(glm::vec3 SetPos,
    float DimX, float DimY, float DimZ)
    : ColShapes(SetPos)
{
    this->XLength = DimX;
    this->YLength = DimY;
    this->ZLength = DimZ;
}

AABB_Obj::~AABB_Obj()
{
}

glm::vec3 AABB_Obj::GetLengths()
{
    return glm::vec3(this->XLength / 2,
        this->YLength / 2,
        this->ZLength / 2);
}

void AABB_Obj::SetLengths(glm::vec3 NewLengths)
{
    this->XLength = NewLengths.x;
    this->YLength = NewLengths.y;
    this->ZLength = NewLengths.z;
}

glm::vec3 CoatlPhysicsEngine::AABB_Obj::GetClosesPoint(glm::vec3 Point)
{
    glm::vec3 P = Point;
    glm::vec3 L = GetLengths();
    glm::vec3 Pos = this->GetPos();
    P.x = this->GetPoint(P.x, Pos.x + L.x, Pos.x - L.x);
    P.y = this->GetPoint(P.y, Pos.y + L.y, Pos.y - L.y);
    P.z = this->GetPoint(P.z, Pos.z + L.z, Pos.z - L.z);
	return P;
}
