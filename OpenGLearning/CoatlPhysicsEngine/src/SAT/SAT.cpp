#include "SAT.h"
using namespace CoatlPhysicsEngine;
SAT::SAT()
{
}

SAT::~SAT()
{
}

glm::vec3 SAT::GetPenetrationContacts(std::shared_ptr<ColShapes> Bod0, std::shared_ptr<ColShapes> Bod1)
{
	std::vector<glm::vec3> Vertex0 = Bod0->GetVertices();
	std::vector<glm::vec3> Vertex1 = Bod1->GetVertices();
	std::vector<glm::vec3> Normals0;
	std::vector<glm::vec3> Normals1;
	
	return glm::vec3();
}
