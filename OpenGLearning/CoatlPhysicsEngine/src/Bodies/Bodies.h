#pragma once
#include <vector>

#include "../Shapes/ColShapes.h"
#include "../Physics/Mass.h"
namespace CoatlPhysicsEngine {
	struct BodyParts
	{
		ColShapes* BodPart;
		Mass* BodMass;
		BodyParts()
		{
			this->BodMass = NULL;
		}
	};
	struct OtherBody
	{
		bool Collided;
		int ID;
	};
	class Bodies
	{
	private:
		int ID;
		std::vector<BodyParts*> BodyInf;		
	public:
		Bodies();
		~Bodies();
		bool DetecCollision(Bodies* OtherBody);
		
	};
}
