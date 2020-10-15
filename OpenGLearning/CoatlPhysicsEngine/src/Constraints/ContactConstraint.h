#pragma once
#include "../CollisionResolution/Contact.h"
namespace CoatlPhysicsEngine {
	class ContactConstraint
	{
		glm::vec3 CreateOrtho(glm::vec3 Norm);
		std::vector<glm::vec3> U_Creation(glm::vec3 Norm);
	public:
		ContactConstraint() {};
		~ContactConstraint() {};
		void SolveContactConstraint(std::vector< std::shared_ptr<Contact>> CntLst);
	};
}
