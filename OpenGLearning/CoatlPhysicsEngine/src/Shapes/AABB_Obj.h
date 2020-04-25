#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine {

	class AABB_Obj :
		public ColShapes
	{
		float XLength;
		float YLength;
		float ZLength;
		float GetPoint(float P, float Max,float Min);		
	public: 
		AABB_Obj(glm::vec3 SetPos,float DimXYZ);
		AABB_Obj(glm::vec3 SetPos,float DimX, float DimYZ);
		AABB_Obj(glm::vec3 SetPos, float DimX, float DimY,float DimZ);
		~AABB_Obj();
		glm::vec3 GetLengths();
		std::vector<glm::vec3> GetPoints();
		std::vector<glm::vec3> GetSegs();
		std::vector<int> GetSegmentsID();
		void SetLengths(glm::vec3 NewLengths);
		glm::vec3 GetClosesPoint(glm::vec3 Point);
	};
}
