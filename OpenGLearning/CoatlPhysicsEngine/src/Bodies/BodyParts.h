#pragma once
#include <vector>
#include "../Shapes/ColShapes.h"
#include "../Physics/Particle.h"
namespace CoatlPhysicsEngine {
	class BodyParts
	{
	private:
		std::shared_ptr<BodyParts> Parent;
		std::shared_ptr<ColShapes> BodPart;
		std::shared_ptr<Particle> BodParticle;
	public:
		BodyParts();
		BodyParts(std::shared_ptr<ColShapes> InitBodPart);
		~BodyParts();
		// General class inits and addition
		void AddShape(std::shared_ptr<ColShapes> InitBodPart);
		void AddParticle(std::shared_ptr<Particle> InitParticle);
		//Body Matrix Information
		glm::mat4 GetParMatrix();
		glm::mat4 GetMatrix();
		void UpdateMatrix();
		//other General information
		glm::vec3 GetPos();
		glm::quat GetQuatAngle();
		void SetPos(glm::vec3 NewPos);
		void SetQuat(glm::quat Q);
		std::shared_ptr<ColShapes> GetShape();
		std::shared_ptr<Particle> GetParticle();
	};
}