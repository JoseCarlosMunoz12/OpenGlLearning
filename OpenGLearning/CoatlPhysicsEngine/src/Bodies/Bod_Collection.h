#pragma once
#include "Joints/Joints.h"
#include "Bodies.h"
namespace CoatlPhysicsEngine {
	class Collection
	{
		std::vector<std::shared_ptr<ColJoints>> Jnts;
	public:
		Collection();
		~Collection();
		void MakeJoint(std::shared_ptr<ColJoints> NewCol);
		void DeleteJoint(int ID);
		std::vector<std::shared_ptr<ColJoints>> GetJoints();
	};	
}