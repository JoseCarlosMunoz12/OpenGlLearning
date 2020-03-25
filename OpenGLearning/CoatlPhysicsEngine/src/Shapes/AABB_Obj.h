#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine {

	class AABB_Obj :
		public ColShapes
	{
		float XLength;
		float YLength;
		float ZLength;
	public: 
		AABB_Obj(glm::vec3 SetPos,float DimXYZ);
		AABB_Obj(glm::vec3 SetPos,float DimX, float DimYZ);
		AABB_Obj(glm::vec3 SetPos, float DimX, float DimY,float DimZ);
		~AABB_Obj();
		glm::vec3 GetLengths();
	};
}
