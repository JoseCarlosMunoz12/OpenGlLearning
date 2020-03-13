#pragma once
#include <string>
#include <iostream>
namespace CoatlPhysicsEngine 
{
	class StaticCollisions
	{
		std::string Name;
	public:
		StaticCollisions(std::string Name);
		~StaticCollisions();
		void ShowName();
	};
}

