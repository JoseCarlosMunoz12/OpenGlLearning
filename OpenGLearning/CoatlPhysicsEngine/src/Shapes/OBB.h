#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine {
	class OBB : public ColShapes
	{
	private:
		float Angle;
		glm::vec3 UnitVec;
		glm::vec3 Ex;
	public:
		//Upright OBB
		OBB(glm::vec3 Pos, float DimXYZ);
		OBB(glm::vec3 Pos, float DimX, float DimYZ);
		OBB(glm::vec3 Pos, float DimX, float DimY, float DimZ);
		//Predefined Quat
		OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimXYZ);
		OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec, float DimX, float DimYZ);
		OBB(glm::vec3 Pos, float InitAngle, glm::vec3 InitUnitVec,float  DimX, float DimY, float DimZ);
		~OBB();
		//Getters
		glm::vec3 GetUnitVec();
		float GetAngle();
		//Setters
		void SetAngle(float SetAngle);
		void SetUnitVec(glm::vec3 SetUnitVec);
		std::vector<glm::vec3> GetSegments();
		glm::vec3 GetLenghts();
		glm::vec3 GetClosestPoint(glm::vec3 Point);
	};
}
