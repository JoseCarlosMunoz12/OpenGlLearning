#pragma once
#include "../Bodies/Bodies.h"
	class Contact
	{
	private:
		glm::mat3 ContactToWorld;
		glm::vec3 ContactVelocity;
		float DesDeltaVel;
		glm::vec3 RelContact[2];
		void CalculateContactbasis();
	public:
		Contact();
		~Contact();
		glm::vec3 ContactPoint;
		glm::vec3 Normal;
		float Penetration;
		float Friction;
		float Restituion;
		void CalculateInternals(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1,float dt);
	};
	struct Manifold
	{
		std::vector<std::shared_ptr<Contact>> Contacts;
		unsigned ContactCount;
		std::shared_ptr<Bodies> Bod0;
		std::shared_ptr<Bodies> Bod1;
		//-1 for both are movable
		//0 For Bod0 is only Movable
		//1 For Bod1 is only Movable
		//2 is for the Joint usage
		int ID;
	};