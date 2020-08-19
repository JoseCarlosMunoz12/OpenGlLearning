#pragma once
#include "../Bodies/Bodies.h"
	class Contact
	{
	private:
		glm::mat3 ContactToWorld;
		glm::vec3 ContactVelocity;
		float DesDeltaVel;
		glm::vec3 RelContact[2];
		glm::mat3 MakeSkew(glm::vec3 Vec);
		void CalculateContactbasis();
		glm::vec3 CalculateLocalvel(std::shared_ptr<Bodies> Bod, int ID, float dt);
		void CalculateDesVel(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1, float dt);
		glm::vec3 CalcFricImpulse(std::shared_ptr<Bodies> Bods[2], glm::mat3 InvInTn[2]);
		glm::vec3 CalcNonFricImpulse(std::shared_ptr<Bodies> Bods[2], glm::mat3 InvInTn[2]);
	public:
		Contact();
		~Contact();
		glm::vec3 ContactPoint;
		glm::vec3 Normal;
		float Penetration;
		float Friction;
		float Restituion;
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