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
		std::vector<glm::vec3> Units = { glm::vec3(0.f, 0.f, 1.f),glm::vec3(0.f, 1.f, 0.f),glm::vec3(1.f, 0.f, 0.f),
										glm::vec3(0.f, 0.f,-1.f),glm::vec3(0.f,-1.f, 0.f),glm::vec3(-1.f, 0.f, 0.f) };
		int ID;
		std::shared_ptr<BodyParts> BodyInf;
		std::vector<int> BoolId;
		glm::vec3 Max;
		glm::vec3 Min;
		glm::vec3 Mid;
	public:
		Bodies(int InitID);
		Bodies(std::shared_ptr<ColShapes> InitShapes, int InitID);
		~Bodies();
		void AddShapes(std::shared_ptr<ColShapes> NewShape);
		int GetID();
		//For base Parents
		void SetPosition(glm::vec3 NewPos);
		void UpdateAABB();
		void MovePosition(glm::vec3 Add);
		void SetParticle(int ShapeID);
		void SetRigidBody(int ShapeID);
		void SetQuat(glm::quat NewQuat);
		void ResetBools() { this->BoolId.clear(); };
		bool HasId(std::shared_ptr<Bodies> OtherBod);
		bool HasCollided();
		void AddId(int NewId);
		void RemoveID(int RevId);
		glm::vec3 GetPos();
		glm::quat GetQuat();
		std::shared_ptr<ColShapes> GetShapes();
		std::shared_ptr<BodyParts> GetBodyParts();
		std::shared_ptr<Bod_Base> GetParticle();
		//get AABB Hierarchy
		glm::vec3 GetMax() { return this->Max; };
		glm::vec3 GetMin() { return this->Min; };
		glm::vec3 GetMid() { return this->Mid; };
	};
}
