#include "ContactConstraint.h"
#include <math.h>
using namespace CoatlPhysicsEngine;

std::vector<glm::vec3> ContactConstraint::U_Creation(glm::vec3 Norm, glm::vec3 DivVel)
{
	std::vector<glm::vec3> Us;
	glm::vec3 NormVel = DivVel * Norm * Norm;
	glm::vec3 TanVel = DivVel - NormVel;
	float T = TanVel.length();
	if (T > std::numeric_limits<float>::epsilon())
		Us.push_back(TanVel / T);
	else
		Us.push_back(glm::normalize(this->CreateOrtho(Norm)));
	Us.push_back(glm::cross(Norm, Us[0]));
	return Us;
}

glm::vec3 ContactConstraint::CreateOrtho(glm::vec3 Norm)
{
	std::vector<float> R;
	R.push_back(Norm.x);
	R.push_back(Norm.y);
	R.push_back(Norm.z);
	int ID = 0;
	for (int ii = 1; ii < 3; ii++)
	{
		if (glm::abs(R[ii]) < glm::abs(R[ID]))
			ID = ii;
	}
	float Den;
	if (ID == 0)
	{
		Den = glm::sqrt(Norm.y * Norm.y + Norm.z * Norm.z);
		return glm::vec3(0.f, -Norm.z / Den, Norm.y / Den);
	}
	else if (ID == 1)
	{
		Den = glm::sqrt(Norm.x * Norm.x + Norm.z * Norm.z);
		return glm::vec3(-Norm.z / Den,0.f, Norm.x / Den);
	}
	Den = glm::sqrt(Norm.x * Norm.x + Norm.y * Norm.y);
	return glm::vec3(-Norm.y / Den, Norm.x / Den, 0.f);
}

void ContactConstraint::SolveContactConstraint(std::vector<std::shared_ptr<Contact>> CntLst)
{
	for (auto& jj : CntLst)
	{
		jj->Bods[0];
	}
}
