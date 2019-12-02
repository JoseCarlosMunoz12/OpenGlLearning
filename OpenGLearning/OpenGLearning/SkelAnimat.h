#pragma once
#include <vector>
#include "Vertex.h"
class Joints
{
private:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Convert(glm::vec3 Rot)
	{
		Rot = Rot / 180.f * glm::pi<float>();
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
	//Getters
	glm::vec3 GetPos()
	{
		return this->Position;
	}
	glm::mat4 GetTransMat()
	{
		glm::mat4 Temp = glm::mat4(1.f);
		return glm::translate(Temp, this->Position);		
	}
	glm::quat GetRot()
	{
		return glm::quat(Convert(this->Rotation));
	}
	glm::vec3 GetRotDeg()
	{
		return this->Rotation;
	}
	//Setters
	void SetPos(glm::vec3 NewOffset)
	{
		this->Position = NewOffset;
	}
	void SetRot(glm::vec3 NewRot)
	{
		this->Rotation = NewRot;
	}
};
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
		this->Joint_Trans = new Joints(InitOffset, InitRot);
	}
	glm::quat ReturnRot()
	{
		return this->Joint_Trans->GetRot();
	}
	glm::vec3 GetOffset()
	{
		return this->Joint_Trans->GetPos();
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
	Frames AnimFrames;
public:
	SkelAn(SkelAn* InitParent,float InitTimeStamp, glm::vec3 InitOffset, glm::vec3 InitRot)
		:AnimFrames(InitTimeStamp,InitOffset,InitRot)
	{

	}
	~SkelAn()
	{

	}
};
