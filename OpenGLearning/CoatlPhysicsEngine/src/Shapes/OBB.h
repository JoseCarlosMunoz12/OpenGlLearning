#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine {
	class OBB : public ColShapes
	{
	private:
		glm::vec3 Ex;
		std::vector<glm::vec3> GetSegOffset(float Ext);
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
		std::vector<glm::vec3> GetSegments();
		glm::vec3 GetLenghts();
		glm::vec3 GetClosestPoint(glm::vec3 Point);
		void SetQuat(glm::quat NewQuat) override;
		glm::vec3 Support(glm::vec3 Dir) override;
		glm::vec3 EPA_Support(glm::vec3 Dir) override;
		std::vector<glm::vec3> GetVertices() override;
		std::vector<glm::vec3> GetNormals() override;
	};
}
