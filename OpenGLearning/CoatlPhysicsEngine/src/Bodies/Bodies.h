#pragma once
#include <vector>

#include "../Shapes/ColShapes.h"
#include "../Physics/Mass.h"
namespace CoatlPhysicsEngine {
	struct BodyParts
	{
		std::shared_ptr<ColShapes> BodPart;
		std::shared_ptr<Mass> BodMass;
		BodyParts()			
		{
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
		glm::vec3 Pos;
		std::vector<std::shared_ptr<BodyParts>> BodyInf;
		std::vector<std::shared_ptr<OtherBody>> OtherInter;
	public:
		Bodies(int InitID);
		Bodies(glm::vec3 NewPos,std::shared_ptr<ColShapes> InitShapes, int InitID);
		Bodies(glm::vec3 NewPos,std::vector<std::shared_ptr<ColShapes>> InitShapes, int InitID);
		~Bodies();
		void AddShapes(ColShapes NewShape);
		bool DetectCollision(std::shared_ptr<Bodies> OtherBody);
		void UpDateBodiesInf(std::shared_ptr<Bodies> Body);
		int GetID();
		void SetPosition(glm::vec3 NewPos);
		std::vector<std::string> AllShapeTypes();
		glm::vec3 GetPos();
	};
}
