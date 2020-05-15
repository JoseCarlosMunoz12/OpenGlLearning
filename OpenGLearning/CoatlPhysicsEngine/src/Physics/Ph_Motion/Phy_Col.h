#pragma once
#include "../../Bodies/Bodies.h"
#include "../../Shapes/ColShapes.h"
#include "../../Shapes/Sphere.h"
#include "../../Shapes/AABB_Obj.h"
#include "../../Shapes/OBB.h"
#include "../../Shapes/Capsule.h"
#include "../../Shapes/Triangles.h"

namespace CoatlPhysicsEngine {
	class Phy_Col
	{
	protected:
		bool Elastic;
		void ElasticCollision(glm::vec3& Vel0, float InvMass0, Sphere Sph0,
								glm::vec3& Vel1, float InvMass1,Sphere Sph1);
	public:
		Phy_Col();
		~Phy_Col();
	};
}