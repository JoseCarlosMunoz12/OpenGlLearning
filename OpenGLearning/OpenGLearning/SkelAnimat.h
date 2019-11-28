#pragma once
#include <glm.hpp>
class SkeletonAnimation
{
private:
	SkeletonAnimation* Parent;
	glm::mat4 BoneMat;
	glm::vec3 Offset;
	glm::vec3 Rotation;
	glm::vec3 Scale;
public:
	SkeletonAnimation(SkeletonAnimation* Parent,
						glm::vec3 InitOffset, glm::vec3 InitRot, glm::vec3 InitScale)
		:BoneMat(glm::mat4(1.f))
	{
		this->Parent = Parent;
		this->Offset = InitOffset;
	}
	~SkeletonAnimation()
	{

	}
};
