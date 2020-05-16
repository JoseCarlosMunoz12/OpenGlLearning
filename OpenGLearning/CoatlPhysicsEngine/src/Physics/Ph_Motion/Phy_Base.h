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
class Phy_Base
{
public:
	Phy_Base();
	~Phy_Base();
	virtual glm::vec3 GetForce() { return glm::vec3(0.f); };
};

