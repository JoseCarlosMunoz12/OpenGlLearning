#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine{
	class Capsule: public ColShapes
	{
		float Radius;
		glm::vec3 APos;
		glm::vec3 BPos;
		//Rotation information
		float Angle;
		glm::vec3 UnitVec;
	public:
		Capsule(glm::vec3 Pos,float InitRadius,float InitLength);
		~Capsule();
		float GetRadius();
		glm::vec3 ClosestPoint_P(glm::vec3 Point);
		float Distance(glm::vec3 Point);
		float Distance(std::vector<glm::vec3> Segment);
		std::vector<glm::vec3> GetSegment();
		//Getter and setters
		float GetAngle();
		glm::vec3 GetUnitVec();
		void SetAngle(float NewAngle);
		void SetVec(glm::vec3 NewVec);
	};
}