#pragma once
#include <vector>
#include "../Shapes/ColShapes.h"
#include "../Physics/Particle.h"
#include "../Physics/RigidBodies.h"
#include "BodyParts.h"
#include "Joints/Joints.h"
namespace CoatlPhysicsEngine {
	class Bodies
	{
	private:
		std::vector<glm::vec3> Units = {glm::vec3(0.f, 0.f, 1.f),glm::vec3(0.f, 1.f, 0.f),glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f, 0.f, -1.f),glm::vec3(0.f, -1.f, 0.f),glm::vec3(-1.f, 0.f, 0.f)};
		int ID;
		int Vec_Size;
		std::vector<std::shared_ptr<BodyParts>> BodyInf;
		std::vector<std::unique_ptr<Joints>> Joints;
		glm::vec3 Max;
		glm::vec3 Min;
		glm::vec3 Mid;
	public:
		Bodies(int InitID);
		Bodies(std::shared_ptr<ColShapes> InitShapes, int InitID);
		Bodies(std::vector<std::shared_ptr<ColShapes>> InitShapes, int InitID);
		~Bodies();
		void AddShapes(std::shared_ptr<ColShapes> NewShape, int Parent);
		int GetID();
		int GetVecSize();
		//For base Parents
		void SetPosition(glm::vec3 NewPos);
		void UpdateAABB();
		void MovePosition(glm::vec3 Add);
		void SetParticle(int ShapeID);
		void SetRigidBody(int ShapeID);
		void SetQuat(glm::quat NewQuat);
		glm::vec3 GetPos();
		glm::quat GetQuat();
		std::shared_ptr<ColShapes> GetShapes();
		std::vector< std::shared_ptr<ColShapes>> GetAllShapes();
		std::vector<std::shared_ptr<BodyParts>> GetBodyParts();
		std::shared_ptr<BodyParts> GetSpecificBodyPart(int ID);
		std::shared_ptr<Bod_Base> GetParticle(int ID);
		//get AABB Hierarchy
		glm::vec3 GetMax() { return this->Max; };
		glm::vec3 GetMin() { return this->Min; };
		glm::vec3 GetMid() { return this->Mid; };
		//Create A joint with the bodies
		void MakeJoint(std::shared_ptr<BodyParts> Bod0, std::shared_ptr<BodyParts> Bod1);
		void MakeJoint(int ID0, int ID1);
	};
}
