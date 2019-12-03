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
	std::vector<Frames*> AnimFrames;
	float GetTimeRatio(float CurrTime, std::vector<Frames*> FrmFound)
	{
		float TimeLeft = CurrTime - FrmFound[0]->GetTimeStamp();
		float FrameDif = FrmFound[1]->GetTimeStamp() - FrmFound[0]->GetTimeStamp();
		return TimeLeft / FrameDif;
	}
	glm::quat Interpolate(glm::quat FirstAngle, glm::quat Secondangle, float Ratio)
	{		
		return glm::mix(FirstAngle, Secondangle, Ratio);
	}
	glm::vec3 AveragePos(glm::vec3 FirstPos, glm::vec3 SecondPos, float Ratio)
	{
		glm::vec3 Temp = FirstPos + SecondPos;
		return Temp * Ratio;
	}
	std::vector<Frames*> GetTwoFrames(float CurrentTime)
	{
		std::vector<Frames*> Vals;
		int Count = 0;
		for (auto& Frm : AnimFrames)
		{
			if (Frm->GetTimeStamp() > CurrentTime)
			{
				break;
			}
			Count++;
		}
		Vals.push_back(AnimFrames[Count - 1]);
		Vals.push_back(AnimFrames[Count]);
		return Vals;
	}
	
public:
	SkelAn(SkelAn* InitParent,float InitTimeStamp, glm::vec3 InitOffset, glm::vec3 InitRot)
		:Parent(InitParent)
	{		

	}
	~SkelAn()
	{

	}
	glm::mat4 GetCurMat(float CurTime)
	{
		std::vector<Frames*> Found = this->GetTwoFrames(CurTime);
		float Ratio = this->GetTimeRatio(CurTime,Found);
		glm::quat newQuat = this->Interpolate(Found[0]->ReturnRot(), Found[1]->ReturnRot(), Ratio);
		glm::vec3 AveragePos = this->AveragePos(Found[0]->GetOffset(), Found[1]->GetOffset(), Ratio);
		glm::mat4 Matrix = glm::translate(glm::mat4(1.f), AveragePos);
		if (Parent)
		{
			return Parent->GetCurMat(CurTime) * Matrix * glm::mat4_cast(newQuat);
		}else{
			return Matrix * glm::mat4_cast(newQuat);
		}		
	}
};
