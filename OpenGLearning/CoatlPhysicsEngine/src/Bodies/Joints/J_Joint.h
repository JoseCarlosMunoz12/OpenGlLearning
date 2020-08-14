#pragma once
#include "Joints.h"
namespace CoatlPhysicsEngine
{
	class J_Joint : public ColJoints
	{
	private:
	public:
		J_Joint(float SetError);
		~J_Joint();
	};
}

