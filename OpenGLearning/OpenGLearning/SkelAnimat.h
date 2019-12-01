#pragma once
#include <vector>
#include "Vertex.h"
class SkelAn
{
private:
	SkelAn* Parent;
	float AnimatLength;
	std::vector<Frames> AnimFrames;
public:
	SkelAn(SkelAn* InitParent, glm::vec3 InitOffset, glm::vec3 InitRot)
	{

	}
	~SkelAn()
	{

	}
};
class Frames
{
	float TimeStamp;
	std::vector<Joints> Joint_Trans;
public:
	Frames(float InitTimeStamp, std::vector<Joints> InitJoints)
	{
		this->TimeStamp = InitTimeStamp;
		this->Joint_Trans = InitJoints;
	}
	Frames(float InitTimeStamp, glm::vec3 InitOffset, glm::vec3 InitRot)
	{
		this->TimeStamp;		
	}
};
class Joints
{
private:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Convert(glm::vec3 Rot)
	{
		Rot.x /= 180.f * glm::pi<float>();
		Rot.y /= 180.f * glm::pi<float>();
		Rot.z /= 180.f * glm::pi<float>();
		return Rot;
	}
public:
	Joints()
	{
		this->Position = glm::vec3(0.f);
		this->Rotation = glm::vec3(0.f);
	}
	Joints(glm::vec3 InitPos, glm::vec3 InitRot)
	{
		this->Position = InitPos;
		this->Rotation = InitRot;
	}
	glm::mat4 GetTrans()
	{
		glm::mat4 Temp = glm::mat4(1.f);
		Temp = glm::translate(Temp, this->Position);
		glm::quat Set = glm::quat(Convert(this->Rotation));
		
	}
};
