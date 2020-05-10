#pragma once
#include <vector>
#include "../Shapes/ColShapes.h"
#include "../Physics/Particle.h"
namespace CoatlPhysicsEngine {
	struct BodyParts
	{
		std::shared_ptr<ColShapes> BodPart;
		std::shared_ptr<Particle> BodPart;
	};
	class Bodies
	{
	private:
		int ID;
		std::vector<std::shared_ptr<BodyParts>> BodyInf;
		std::vector<int> OtherBods;
	public:
		Bodies(int InitID);
		Bodies(std::shared_ptr<ColShapes> InitShapes, int InitID);
		Bodies(std::vector<std::shared_ptr<ColShapes>> InitShapes, int InitID);
		~Bodies();
		void AddShapes(std::shared_ptr<ColShapes> NewShape);
		void UpDateBodiesInf(bool ColUPdate,std::shared_ptr<Bodies> Body);
		void ClearColsInf();
		int GetID();
		void CalcPhysics();
		void SetPosition(glm::vec3 NewPos);
		void SetParticle(int ShapeID);
		glm::vec3 GetPos();
		std::shared_ptr<ColShapes> GetShapes();
		std::vector<int> GetAllCol();
		std::vector<std::shared_ptr<BodyParts>> GetBodyParts();
		std::shared_ptr<BodyParts> GetSpecificBodyPart(int ID);
	};
}
