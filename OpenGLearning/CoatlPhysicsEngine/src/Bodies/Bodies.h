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
			this->BodPart = NULL;
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
		std::vector<OtherBody*> OtherInter;
	public:
		Bodies();
		~Bodies();
		bool DetectCollision(Bodies* OtherBody);
		void UpDateBodiesInf(Bodies* Body);
		
	};
}
