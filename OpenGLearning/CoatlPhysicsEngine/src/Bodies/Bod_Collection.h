#pragma once
#include "Joints/Joints.h"
namespace CoatlPhysicsEngine {
	class Collection
	{
		std::vector<std::shared_ptr<ColJoints>> Jnts;
	public:
		Collection();
		~Collection();
		void MakeJoint();
		void DeleteJoint(int ID);
	};	
}