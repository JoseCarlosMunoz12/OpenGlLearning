#pragma once
#include <vector>
#include "Vertex.h"
#include <map> 
#include <algorithm>
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
	QuatParts ReturnRot()
	{
		return this->Joint_Trans.Rot;
	}
	glm::vec3 GetOffset()
	{
		return this->Joint_Trans.Offset;
	}
	glm::vec3 GetScale()
	{
		return this->Joint_Trans.Scale;
	}
	float GetTimeStamp()
	{
		return this->TimeStamp;
	}
	void SetRot(QuatParts NewRot)
	{
		this->Joint_Trans.Rot = NewRot;
	}
	void NewOffset(glm::vec3 NewOffset)
	{
		this->Joint_Trans.Offset = NewOffset;
	}
};
class SkelAn
{
private:
	std::string ParentId;
	std::vector<Frames*> AnimFrames;
	glm::vec3 CurOffset;
	glm::vec3 CurScale;
	QuatParts CurRot;
	glm::mat4 Matrix;
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
		Vals.push_back(AnimFrames[Count]);
		Vals.push_back(AnimFrames[Count + 1]);
		return Vals;
	}
public:
	SkelAn(std::vector<Frames*> InitFrames, std::string ParentName,glm::vec3 InitOffset,
		QuatParts InitQuat = QuatParts(),glm::vec3 InitScale = glm::vec3(1.f))
		:CurOffset(InitOffset),CurScale(InitScale),CurRot(InitQuat)
	{
		this->ParentId = ParentName;
		this->AnimFrames = InitFrames;
		this->Matrix = glm::mat4(1.f);
	}	
	~SkelAn()
	{

	}
	glm::mat4 GetCurMat(std::map<std::string, SkelAn*> Temp,float CurTime)
	{
		std::vector<Frames*> Found = this->GetTwoFrames(CurTime);
		float Ratio = this->GetTimeRatio(CurTime, Found);
		glm::quat newQuat = this->Interpolate(Found[0]->ReturnRot().GetQuat(), Found[1]->ReturnRot().GetQuat(), Ratio);
		glm::vec3 AveragePos = this->AveragePos(Found[0]->GetOffset(), Found[1]->GetOffset(), Ratio);
		glm::mat4 Matrix = glm::translate(glm::mat4(1.f), AveragePos);
		if (ParentId != "NULL")
		{
			return Temp[ParentId]->GetCurMat(Temp, CurTime) * Matrix * glm::mat4_cast(newQuat);
		}
		else {
			return Matrix * glm::mat4_cast(newQuat);
		}		
	}
	glm::mat4 GetMat(std::map<std::string, SkelAn*> Temp)
	{
		if (ParentId != "NULL")
		{
			return Temp[ParentId]->GetMat(Temp) * this->Matrix ;
		}
		else {
			return this->Matrix;
		}		
	}
	std::string GetName()
	{
		return this->ParentId;
	}
	QuatParts GetRot()
	{
		return this->CurRot;
	}
	glm::vec3 GetOffset()
	{
		return this->CurOffset;
	}
	glm::vec3 GetScale()
	{
		return this->CurScale;
	}
	void SetRot(QuatParts NewRot)
	{
		this->CurRot = NewRot;
	}
	void SetOffset(glm::vec3 NewOffset)
	{
		this->CurOffset = NewOffset;
	}
	void SetScale(glm::vec3 NewScale)
	{
		this->CurScale = NewScale;
	}
	void UpdateMatrix()
	{
		Matrix = glm::mat4(1.f);
		Matrix = glm::translate(Matrix,this->CurOffset);	
		glm::mat4 Quats = glm::mat4_cast(this->CurRot.GetQuat());
		Matrix *= Quats;
		Matrix = glm::translate(Matrix, -this->CurOffset);
		Matrix = glm::scale(Matrix, this->CurScale);
	
	}
};
