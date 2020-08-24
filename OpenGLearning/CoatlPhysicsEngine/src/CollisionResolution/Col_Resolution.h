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
#include "Col_Resolution.h"
#include "../GJK/GJK.h"
#include "../SAT/SAT.h"
namespace CoatlPhysicsEngine {
	class Col_Resolution;
	class Contacts {
		friend class Col_Resolution;
	private:
		glm::mat3 MakeSkew(glm::vec3 Vec);
		void CalculateContactbasis();
		glm::vec3 CalculateLocalvel( int ID, float dt);
		glm::vec3 CalcFricImpulse(glm::mat3 InvInTn[2]);
		glm::vec3 CalcNonFricImpulse( glm::mat3 InvInTn[2]);
		void CalculateDesVel(float dt);
		void MatchAwakeState();
		void CalculateInternals( float dt);
		//applies changes and resolutions
		void ApplyPositionChange(glm::vec3 LinChang[2], glm::vec3 AngChange[2]);
		void ApplyVelocityChange(glm::vec3 VelChang[2], glm::vec3 RotChange[2]);
		glm::mat3 ContactToWorld;
		glm::vec3 ContactVelocity;
		float DesDeltaVel;
		glm::vec3 RelContact[2];
	public:
		std::shared_ptr<Bodies> Bods[2];
		glm::vec3 ContactPoint;
		glm::vec3 Normal;
		float Penetration;
		float Friction;
		float Restituion;
	};
	class Col_Resolution
	{
	private:
		//Iterations Count
		int VelIt;
		int Posit;
		// Impulse resolition
		void PrepeareContact(std::vector<std::shared_ptr<Contacts>> Cnt, float dt);
		void AdjustPosition(std::vector<std::shared_ptr<Contacts>> Cnt, float dt);
		void AdjustVelocity(std::vector<std::shared_ptr<Contacts>> Cnt, float dt);
		//Contact creation
		std::vector<std::shared_ptr<Contacts>> ContactCreate(Sphere Sph0, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::vector<std::shared_ptr<Contacts>> ContactCreate(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::vector<std::shared_ptr<Contacts>> ContactCreate(Capsule Cap, std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
		std::unique_ptr<ShapeResolution> S_Res;
		std::vector<std::shared_ptr<Contacts>> MakeContacts(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1);
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