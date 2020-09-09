#pragma once
#include <vector>
#include "../Shapes/ColShapes.h"
#include "../Physics/Bod_Base.h"
namespace CoatlPhysicsEngine {
	class BodyParts
	{
	private:
		std::shared_ptr<ColShapes> BodPart;
		std::shared_ptr<Bod_Base> BodParticle;
	public:
		BodyParts();
		BodyParts(std::shared_ptr<ColShapes> InitBodPart);
		~BodyParts();
		// General class inits and addition
		void AddShape(std::shared_ptr<ColShapes> InitBodPart);
		void AddParticle(std::shared_ptr<Bod_Base> InitParticle);
		//Body Matrix Information
		//other General information
		glm::vec3 GetPos();
		glm::quat GetQuatAngle();
		void SetPos(glm::vec3 NewPos);
		void SetQuat(glm::quat Q);
		std::shared_ptr<ColShapes> GetShape();
		std::shared_ptr<Bod_Base> GetParticle();
		glm::vec3 Support(glm::vec3 Dir);
	};
}