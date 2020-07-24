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
#include "../Bod_Base.h"
namespace CoatlPhysicsEngine {
	class Phy_Base
	{
	public:
		Phy_Base() {

		}
		~Phy_Base() {

		}
		virtual glm::vec3 GetForce(Bod_Base P0, Bod_Base P1) =0;
		virtual glm::vec3 GetForce(Bod_Base P0) = 0;
		virtual glm::vec3 GetForce(Bod_Base P0, float dt) = 0;
	};
}