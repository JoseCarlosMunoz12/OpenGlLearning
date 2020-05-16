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
#include "../Particle.h"
namespace CoatlPhysicsEngine {
	class Phy_Base
	{
	public:
		Phy_Base() {

		}
		~Phy_Base() {

		}
		virtual glm::vec3 GetForce(Particle P0, Particle P1) =0;
	};
}