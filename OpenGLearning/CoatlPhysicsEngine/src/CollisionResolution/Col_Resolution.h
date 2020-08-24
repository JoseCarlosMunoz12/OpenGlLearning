#pragma once
#include <string>
#include <iostream>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>
#include <vector>
#include "../Bodies/Bodies.h"
#include "../Bodies/Bod_Collection.h"
#include "ShapesResolution/ShapeResolution.h"
#include "Manifoldsh.h"
#include "../GJK/GJK.h"
#include "../SAT/SAT.h"
namespace CoatlPhysicsEngine {
	class Col_Resolution;
	class Contacts {
		friend class Col_Resolution;
	private:

	public:

	};
	class Col_Resolution
	{
	private:
		//Iterations Count
		int VelIt;
		int Posit;
		// Impulse resolition
		void PrepeareContact(std::shared_ptr<Manifold> Cnt, float dt);
		void AdjustPosition(std::shared_ptr<Manifold> Cnt, float dt);
		void AdjustVelocity(std::shared_ptr<Manifold> Cnt, float dt);
		//Resolvoing Contacts
		void ResolveResolution(std::shared_ptr<Bodies> Bod, std::shared_ptr<Manifold> Cnt);
		//Contact creation
		std::vector<std::shared_ptr<Contact>> ContactCreate(Sphere Sph0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::vector<std::shared_ptr<Contact>> ContactCreate(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::vector<std::shared_ptr<Contact>> ContactCreate(Capsule Cap, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::unique_ptr<ShapeResolution> S_Res;
		std::vector<std::shared_ptr<Contact>> MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::unique_ptr<GJK_Alg> GJK_;
		std::unique_ptr<SAT> SAT_;
	public:
		Col_Resolution(int VelInit, int PosInit);
		~Col_Resolution();
		//Create Manifolds
		 std::shared_ptr<Manifold> MakeManifold(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1,int ID);
		 void MakeJointManifold(std::vector<std::shared_ptr<Manifold>> &R,std::vector<std::shared_ptr<ColJoints>> BodJoints);
		//Resolve Manifolds
		 void ResolveContacts(std::shared_ptr<Manifold> Contact, float dt);
	};
}