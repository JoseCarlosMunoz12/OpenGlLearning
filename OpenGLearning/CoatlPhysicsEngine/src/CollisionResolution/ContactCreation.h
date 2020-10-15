#pragma once
#include "../Bodies/Bodies.h"
#include "../GJK/GJK.h"
#include "../SAT/SAT.h"
#include "Col_Resolution.h"
#include "ShapesResolution/ShapeResolution.h"
namespace CoatlPhysicsEngine
{
	class ContactCreation
	{
		//Contact creation
		std::unique_ptr<ShapeResolution> S_Res;
		std::vector<std::shared_ptr<Contact>> ContactCreate(Sphere Sph0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::vector<std::shared_ptr<Contact>> ContactCreate(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::unique_ptr<GJK_Alg> GJK_;
		std::unique_ptr<SAT> SAT_;
		std::vector<std::shared_ptr<Contact>> ContactCreate(Capsule Cap, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::vector<std::shared_ptr<Contact>> MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		float GetLowestFric(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		float GetLowestRest(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
	public:
		ContactCreation();
		~ContactCreation();
		//Create Manifolds
		std::vector<std::shared_ptr<Contact>> MakeManifold(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1, float dt0, float dt1);
		void MakeJointManifold(std::vector<std::shared_ptr<Contact>>& R, std::vector<std::shared_ptr<ColJoints>> BodJoints);
	};
}
