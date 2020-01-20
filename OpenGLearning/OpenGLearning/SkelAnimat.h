#pragma once
#include <vector>
#include "Vertex.h"
#include <map> 
#include <algorithm>
class Frames
{ 
	glm::vec3 Convert(glm::vec3 Rot)
	{

	}
	float TimeStamp;	
	Joints Joint_Trans;
public:
	Frames(float InitTimeStamp, Joints InitJoints)
	{
		this->TimeStamp = InitTimeStamp;
		this->Joint_Trans = InitJoints;
	}
	QuatParts GetRot()
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
	QuatParts Interpolate(QuatParts FirstAngle, QuatParts Secondangle, float Ratio)
	{
		float NewAngle = FirstAngle.Angle + (Secondangle.Angle - FirstAngle.Angle) * Ratio;
		glm::vec3 NewVec = FirstAngle.UnitVec * (1 - Ratio) + Secondangle.UnitVec * Ratio;
		return QuatParts(NewAngle,NewVec);
	}

	glm::vec3 AveragePos(glm::vec3 FirstPos, glm::vec3 SecondPos, float Ratio)
	{
		glm::vec3 Temp = FirstPos + SecondPos;
		return Temp * Ratio;
	}
	std::vector<Frames*> GetTwoFrames(float CurrentTime)
	{
		int Count = 0;
		for (auto& Frm : AnimFrames)
		{
			if (Frm->GetTimeStamp() > CurrentTime)
			{
				break;
			}
			Count++;
		}

		if (Count = AnimFrames.size())
		{
			return { AnimFrames[Count - 1],AnimFrames[Count-1] };
		}
		return {AnimFrames[Count-1],AnimFrames[Count]};
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
		if (AnimFrames.size() != 0)
		{
			std::vector<Frames*> Found = this->GetTwoFrames(CurTime);
			float Ratio = this->GetTimeRatio(CurTime, Found);
			this->CurRot = this->Interpolate(Found[0]->GetRot(), Found[1]->GetRot(), Ratio);
		}
		this->UpdateMatrix();
		if (ParentId != "NULL")
		{
			return Temp[ParentId]->GetMat(Temp) * this->Matrix ;
		}
		else {
			return this->Matrix;
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
	std::vector<Frames*> GetFrames()
	{
		return this->AnimFrames;
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
