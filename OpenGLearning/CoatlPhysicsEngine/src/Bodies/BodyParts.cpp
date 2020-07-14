#include "BodyParts.h"
using namespace CoatlPhysicsEngine;

BodyParts::BodyParts()
    :ParMatrix(glm::mat4(1.f))
{

}

BodyParts::~BodyParts()
{
}

glm::mat4 BodyParts::GetParMatrix()
{
    return ParMatrix;
}

void BodyParts::UpdateMatrix()
{

}
