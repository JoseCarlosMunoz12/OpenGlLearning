#pragma once
#include "../Bodies/Bodies.h"
	class Contact
	{
	private:
		glm::mat3 MakeSkew(glm::vec3 Vec);
		void CalculateContactbasis();
		glm::vec3 CalculateLocalvel(std::shared_ptr<Bodies> Bod, int ID, float dt);
		glm::vec3 CalcFricImpulse(std::shared_ptr<Bodies> Bods[2], glm::mat3 InvInTn[2]);
		glm::vec3 CalcNonFricImpulse(std::shared_ptr<Bodies> Bods[2], glm::mat3 InvInTn[2]);
	public:
		Contact();
		~Contact();
		void CalculateDesVel(std::shared_ptr<Bodies> Bods[2], float dt);
		glm::mat3 ContactToWorld;
		glm::vec3 ContactPoint;
		glm::vec3 Normal;
		glm::vec3 ContactVelocity;
		float Penetration;
		float DesDeltaVel;
		float Friction;
		float Restituion;
		glm::vec3 RelContact[2];
		void MatchAwakeState(std::shared_ptr<Bodies> Bods[2]);
		void CalculateInternals(std::shared_ptr<Bodies> Bods[2],float dt);
		//applies changes and resolutions
		void ApplyPositionChange(std::shared_ptr<Bodies> Bods[2],
			glm::vec3 LinChang[2],glm::vec3 AngChange[2]);
		void ApplyVelocityChange(std::shared_ptr<Bodies> Bods[2],
			glm::vec3 VelChang[2], glm::vec3 RotChange[2]);
	};
	struct Manifold
	{
		std::vector<std::shared_ptr<Contact>> Contacts;
		unsigned ContactCount;
		std::shared_ptr<Bodies> Bods[2];
		//-1 for both are movable
		//0 For Bod0 is only Movable
		//1 For Bod1 is only Movable
		//2 is for the Joint usage
		int ID;
	};