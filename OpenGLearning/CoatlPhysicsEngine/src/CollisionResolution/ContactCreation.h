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
		std::vector<std::shared_ptr<Contacts>> ContactCreate(Sphere Sph0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::vector<std::shared_ptr<Contacts>> ContactCreate(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::vector<std::shared_ptr<Contacts>> ContactCreate(Capsule Cap, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::unique_ptr<ShapeResolution> S_Res;
		std::vector<std::shared_ptr<Contacts>> MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::unique_ptr<GJK_Alg> GJK_;
		std::unique_ptr<SAT> SAT_;
	public:
		ContactCreation();
		~ContactCreation();
		//Create Manifolds
		std::vector<std::shared_ptr<Contacts>> MakeManifold(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1, int ID);
		void MakeJointManifold(std::vector<std::shared_ptr<Contacts>>& R, std::vector<std::shared_ptr<ColJoints>> BodJoints);
	};
}
