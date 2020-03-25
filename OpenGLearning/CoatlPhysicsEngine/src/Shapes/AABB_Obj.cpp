#include "AABB_Obj.h"
using namespace CoatlPhysicsEngine;

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
    return glm::vec3(this->XLength,this->YLength,this->ZLength);
}

void AABB_Obj::SetLengths(glm::vec3 NewLengths)
{
    this->XLength = NewLengths.x;
    this->YLength = NewLengths.y;
    this->ZLength = NewLengths.z;
}
