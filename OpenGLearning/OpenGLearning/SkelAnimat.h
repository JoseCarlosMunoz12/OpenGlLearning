#pragma once
#include <vector>
#include "Vertex.h"
class Frames
{
	float TimeStamp;
	Joints Joint_Trans;
public:
	Frames(float InitTimeStamp, Joints InitJoints)
	{
		this->TimeStamp = InitTimeStamp;
		this->Joint_Trans = InitJoints;
	}
	glm::quat ReturnRot()
	{
		return this->Joint_Trans.GetRot();
	}
	glm::vec3 GetRot()
	{
		return this->Joint_Trans.Rotation;
	}
	glm::vec3 GetOffset()
	{
		return this->Joint_Trans.Offset;
	}
	float GetTimeStamp()
	{
		return this->TimeStamp;
	}
	void SetRot(glm::vec3 NewRot)
	{
		this->Joint_Trans.Rotation = NewRot;
	}
	void NewOffset(glm::vec3 NewOffset)
	{
		this->Joint_Trans.Offset = NewOffset;
	}

};
class SkelAn
{
private:
	SkelAn* Parent;
	float AnimatLength;
	std::vector<Frames*> AnimFrames;
	float GetTimeRatio(float CurrTime)
	{

	}
	glm::quat Interpolate(glm::quat FirstAngle, glm::quat Secondangle, float Ratio)
	{		
		return glm::mix(FirstAngle, Secondangle, Ratio);
	}
public:
	SkelAn(SkelAn* InitParent,float InitTimeStamp, glm::vec3 InitOffset, glm::vec3 InitRot)
	{

	}
	~SkelAn()
	{

	}
};
