#include "Manifoldsh.h"

void Contact::CalculateContactbasis()
{
}

Contact::Contact()
	:ContactPoint(glm::vec3(0.f)), Normal(glm::vec3(0.f, 0.f, 1.f)),
	Penetration(0.f), Friction(1.f),Restituion(1.f)
{
}

Contact::~Contact()
{
}

void Contact::CalculateInternals(std::shared_ptr<Bodies> Bod0, std::shared_ptr<Bodies> Bod1, float dt)
{

}
