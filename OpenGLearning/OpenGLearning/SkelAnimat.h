#pragma once
#include <vector>
#include "Vertex.h"
class Frames
{
	float TimeStamp;
	Joints* Joint_Trans;
public:
	Frames(float InitTimeStamp, Joints* InitJoints)
	{
		this->TimeStamp = InitTimeStamp;
		this->Joint_Trans = InitJoints;
	}
	Frames(float InitTimeStamp, glm::vec3 InitOffset, glm::vec3 InitRot)
	{
		this->TimeStamp;
		this->Joint_Trans = new Joints{InitOffset,InitRot};
	}
	glm::quat ReturnRot()
	{
		return this->Joint_Trans->GetRot();
	}
	glm::vec3 GetOffset()
	{
		return this->Joint_Trans->Offset;
	}
	float GetTimeStamp()
	{
		return this->TimeStamp;
	}

};
class SkelAn
{
private:
	SkelAn* Parent;
	float AnimatLength;
	std::vector<Frames*> AnimFrames;
public:
	SkelAn(SkelAn* InitParent,float InitTimeStamp, glm::vec3 InitOffset, glm::vec3 InitRot)
	{

	}
	~SkelAn()
	{

	}
};
