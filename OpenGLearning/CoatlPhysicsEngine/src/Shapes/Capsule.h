#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine{
	class Capsule: public ColShapes
	{
		float Radius;
		glm::vec3 APos;
		glm::vec3 BPos;
		//Rotation information
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
		glm::vec3 GetUnitVec();
		void SetVec(glm::vec3 NewVec);
		void SetQuat(glm::quat NewQuat) override;
		glm::vec3 Support(glm::vec3 Dir) override;
		glm::vec3 EPA_Support(glm::vec3 Dir) override;
	};
}