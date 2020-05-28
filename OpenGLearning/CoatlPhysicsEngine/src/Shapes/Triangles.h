#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine{

	class Triangles : public ColShapes
	{
	private:
		glm::vec3 Points[3];
	public:
		Triangles(glm::vec3 InitPos,std::vector<glm::vec3> InitPoints);
		~Triangles();
		std::vector<glm::vec3> GetSegments();
		glm::vec3 GetClosestPoint(glm::vec3 Point);
		void SetQuat(glm::quat NewQuat) override;
	};

}
