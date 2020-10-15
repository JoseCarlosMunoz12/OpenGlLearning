#pragma once
#include "../CollisionResolution/Contact.h"
namespace CoatlPhysicsEngine {
	class ContactConstraint
	{
	public:
		ContactConstraint() {};
		~ContactConstraint() {};
		void SolveContactConstraint(std::vector<Contact> CntLst);

	};
}
