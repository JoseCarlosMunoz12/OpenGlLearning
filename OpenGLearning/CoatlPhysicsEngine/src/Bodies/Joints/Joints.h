#pragma once
#include "../BodyParts.h"
namespace CoatlPhysicsEngine
{
	class ColJoints
	{
	protected:
		std::weak_ptr<BodyParts> BodA;
		std::weak_ptr<BodyParts> BodB;
	public:
		ColJoints() {};
		~ColJoints() {};
	};
}