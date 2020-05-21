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
#include "../Bodies/Bodies.h"
#include "ShapesResolution/ShapeResolution.h"
#include "Manifoldsh.h"
namespace CoatlPhysicsEngine {
	class Col_Resolution
	{
	private:
		std::unique_ptr<ShapeResolution> S_Res;
		//
		std::vector<std::shared_ptr<Contact>> MakeContacts(Sphere Sph0, std::shared_ptr<Bodies> Bod0);
		//
		std::vector<std::shared_ptr<Contact>> MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
	public:
		Col_Resolution();
		~Col_Resolution();
		 std::shared_ptr<Manifold> MakeManifold(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
	};
}