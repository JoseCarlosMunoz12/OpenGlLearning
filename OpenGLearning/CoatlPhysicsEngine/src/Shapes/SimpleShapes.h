#pragma once
#include "ColShapes.h"
namespace CoatlPhysicsEngine {
	struct Plane
	{
		glm::vec3 Normal;
		float D;
	};
	struct C_Plane
	{
		glm::vec3 Normal;
		float Bias;
		C_Plane(std::vector<glm::vec3> TR)
		{
			Normal = glm::cross((TR[1]-TR[0]), (TR[2] - TR[1]));
			Bias = glm::dot(Normal, TR[1]);
		}
		bool SameLocPl(std::vector<glm::vec3> Pnt)
		{
			int Count = 0;
			std::vector<float> Negs;
			std::vector<float> Pos;
			for (auto& jj : Pnt)
			{
				float Temp = glm::dot(Normal, jj) - Bias;
				if (Temp <= 0)
				{
					Negs.push_back(Temp);
				}
				else
				{
					Pos.push_back(Temp);
				}
				Count++;
				int S = Negs.size();
				int D =  Pos.size();
				int Val = glm::abs(S - D);
				if (Val != Count)
				{
					return false;
				}
			}

			return true;
		}
	};
}