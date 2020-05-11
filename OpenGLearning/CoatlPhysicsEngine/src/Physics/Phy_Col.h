#pragma once
#include <string>
#include <iostream>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
namespace CoatlPhysicsEngine {
	class Phy_Col
	{
	protected:
		bool Elastic;
		void ElasticCollision(glm::vec3& Vel0, float InvMass0,
								glm::vec3& Vel1, float InvMass1);
	public:
		Phy_Col();
		~Phy_Col();
	};
}