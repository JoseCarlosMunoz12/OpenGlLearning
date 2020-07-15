#pragma once
#include <vector>
#include "../Shapes/ColShapes.h"
#include "../Physics/Particle.h"
#include "BodyParts.h"
namespace CoatlPhysicsEngine {
	class Bodies
	{
	private:
		int ID;
		int Vec_Size;
		std::vector<std::shared_ptr<BodyParts>> BodyInf;
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
		void MovePosition(glm::vec3 Add);
		void SetParticle(int ShapeID);
		void SetQuat(glm::quat NewQuat);
		glm::vec3 GetPos();
		glm::quat GetQuat();
		std::shared_ptr<ColShapes> GetShapes();
		std::vector<std::shared_ptr<BodyParts>> GetBodyParts();
		std::shared_ptr<BodyParts> GetSpecificBodyPart(int ID);
	};
}
