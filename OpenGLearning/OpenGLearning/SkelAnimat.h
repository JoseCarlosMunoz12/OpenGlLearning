#pragma once
#include <vector>
#include "Vertex.h"
#include <map> 
#include <algorithm>
enum InterType
{
	HOLD,
	LINEAR,
	QUADBENZ,
	CUBEBENZ
};
class Frames
{ 
	float TimeStamp;
	Bezier_Bais Bias;
	Joints Joint_Trans;
	InterType Type;
public:
	Frames(float InitTimeStamp, Joints InitJoints, InterType InitType = LINEAR)
	{
		this->TimeStamp = InitTimeStamp;
		this->Joint_Trans = InitJoints;
		this->Bias = {0.f,0.f};
		this->Type = InitType;
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
	Bezier_Bais GetBezier()
	{
		return this->Bias;
	}
	InterType GetType()
	{
		return this->Type;
	}
	float GetTimeStamp()
	{
		return this->TimeStamp;
	}
	void SetRot(QuatParts NewRot)
	{
		this->Joint_Trans.Rot = NewRot;
	}
	void SetOffset(glm::vec3 NewOffset)
	{
		this->Joint_Trans.Offset = NewOffset;
	}
	void SetBezier(Bezier_Bais NewBias)
	{
		this->Bias = NewBias;
	}
	void SetType(InterType NewType)
	{
		this->Type = NewType;
	}
};
class SkelAn
{
private:
	std::string ParentId;
	std::vector<Frames*> AnimFrames;
	//Offset Matricex
	glm::vec3 CurOffset;
	glm::vec3 CurScale;
	QuatParts CurRot;
	glm::quat Curquat;
	glm::mat4 OffSet;
	glm::mat4 TransMat;
	glm::mat4 Matrix;
	glm::mat4 RelMat;
	//Different Interpolations
	QuatParts LinInter(QuatParts FirstAngle, QuatParts Secondangle, float Ratio)
	{
		float NewAngle = FirstAngle.Angle + (Secondangle.Angle - FirstAngle.Angle) * Ratio;
		glm::vec3 NewVec = FirstAngle.UnitVec * (1 - Ratio) + Secondangle.UnitVec * Ratio;
		this->Curquat = glm::slerp(FirstAngle.GetQuat(), Secondangle.GetQuat(), Ratio);
		return QuatParts(NewAngle,NewVec);
	}
	QuatParts QuadBezInter(QuatParts FirstAngle, QuatParts SecondAngle, float Ratio, Bezier_Bais Bias)
	{
		float BezAngle0 = (1 - Ratio) * ((1 - Ratio) * FirstAngle.Angle + Ratio * Bias.Point0);
		float BezAngle1 = Ratio * ((1-Ratio) * Bias.Point0 + Ratio*SecondAngle.Angle);
		float NewAngle = BezAngle0 + BezAngle1;
		glm::vec3 NewVec = FirstAngle.UnitVec * (1 - Ratio) + SecondAngle.UnitVec * Ratio;
		return QuatParts(NewAngle, NewVec);
	}
	QuatParts CubeBexInter(QuatParts FirstAngle, QuatParts SecondAngle, float Ratio, Bezier_Bais Bias)
	{
		float Const = glm::pow(1-Ratio,3)*FirstAngle.Angle + 3*glm::pow(1-Ratio,2) * Ratio*Bias.Point0
			+3*(1-Ratio) * glm::pow(Ratio,2) *Bias.Point1 + glm::pow(Ratio,3) *SecondAngle.Angle;
		glm::vec3 NewVec = FirstAngle.UnitVec * (1 - Ratio) + SecondAngle.UnitVec * Ratio;
		return QuatParts(Const, NewVec);
	}
	glm::vec3 VecLinInter(glm::vec3 FirstPos,glm::vec3 SecondPos,float Ratio)
	{
		glm::vec3 NewVec = FirstPos * (1 - Ratio) + SecondPos * Ratio;
		return NewVec;
	}
	//Ratios and In between Frames
	float GetTimeRatio(float CurrTime, std::vector<Frames*> FrmFound)
	{
		float TimeLeft = CurrTime - FrmFound[0]->GetTimeStamp();
		float FrameDif = FrmFound[1]->GetTimeStamp() - FrmFound[0]->GetTimeStamp();
		if (FrameDif == 0)
		{
			return 0;
		}
		return TimeLeft / FrameDif;
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

		if (Count == AnimFrames.size())
		{
			return { AnimFrames[Count - 1],AnimFrames[Count-1] };
		}
		if (Count == 0)
		{
			return{ AnimFrames[Count],AnimFrames[Count+1] };
		}
		return {AnimFrames[Count-1],AnimFrames[Count]};
	}
public:
	SkelAn(std::vector<Frames*> InitFrames, std::string ParentName,glm::mat4 InitMat,glm::mat4 IOffset,glm::vec3 InitOffset,
		QuatParts InitQuat = QuatParts(),glm::vec3 InitScale = glm::vec3(1.f))
		:CurOffset(InitOffset),CurScale(InitScale),CurRot(InitQuat),TransMat(InitMat)
	{
		this->ParentId = ParentName;
		this->AnimFrames = InitFrames;
		this->Matrix = InitMat;
		this->OffSet = IOffset;
	}	
	~SkelAn()
	{

	}
	//Updating Matrices
	glm::mat4 GetCurMat(std::map<std::string, SkelAn*> Temp,float CurTime)
	{
		if (AnimFrames.size() != 0)
		{
			std::vector<Frames*> Found = this->GetTwoFrames(CurTime);
			float Ratio = this->GetTimeRatio(CurTime, Found);
			switch (Found[0]->GetType())
			{
			case HOLD:
				this->CurRot = Found[0]->GetRot();
				this->CurOffset = Found[0]->GetOffset();
				break;
			case LINEAR:
				this->CurRot = this->LinInter(Found[0]->GetRot(), Found[1]->GetRot(), Ratio);
				this->CurOffset = this->VecLinInter(Found[0]->GetOffset(), Found[1]->GetOffset(), Ratio);
				break;
			case QUADBENZ:
				this->CurRot = this->QuadBezInter(Found[0]->GetRot(), Found[1]->GetRot(), Ratio, Found[0]->GetBezier());
				break;
			case CUBEBENZ:
				this->CurRot = this->CubeBexInter(Found[0]->GetRot(), Found[1]->GetRot(), Ratio, Found[0]->GetBezier());
				break;
			}
			this->UpdateRelMat(true, Temp);			
		}
		else
		{
			this->Matrix = this->TransMat;
			this->UpdateRelMat(false, Temp);
		}
		return this->GetAllMats();	
	}
	glm::mat4 GetRelativeMat()
	{
		return this->RelMat;
	}
	glm::mat4 GetAllMats()
	{
		return this->RelMat * this->OffSet;
	}
	void UpdateMatrix()
	{
		Matrix = glm::mat4(1.f);
		Matrix = glm::translate(Matrix,this->CurOffset);	
		glm::mat4 Quats = glm::mat4_cast(this->Curquat);
		Matrix *= Quats;
		Matrix = glm::scale(Matrix, this->CurScale);
	}
	void UpdateRelMat(bool Active, std::map<std::string, SkelAn*> Temp)
	{
		if (Active)
		{
			this->UpdateMatrix();
		}
		if (ParentId == "NULL")
		{
			this->RelMat = this->Matrix;
		}
		else {
			this->RelMat = Temp[ParentId]->GetRelativeMat() * this->Matrix;
		}
	}
	//Getters
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
	glm::mat4 GetOffsetMat()
	{
		return this->OffSet;
	}
	glm::mat4 GetTransmat()
	{
		return this->TransMat;
	}

	//Setters
	void SetRot(QuatParts NewRot)
	{
		this->CurRot = NewRot;
		this->Curquat = this->CurRot.GetQuat();
	}
	void SetOffset(glm::vec3 NewOffset)
	{
		this->CurOffset = NewOffset;
	}
	void SetScale(glm::vec3 NewScale)
	{
		this->CurScale = NewScale;
	}
	void DeleteFrame(int FrameCount)
	{
		this->AnimFrames.erase(this->AnimFrames.begin() + FrameCount);
	}
	void AddFrame(QuatParts NewQuat, glm::vec3 NewOffset, glm::vec3 NewScale,float NewTime)
	{
		int Count = 0;
		for (auto& Frm : AnimFrames)
		{
			if (Frm->GetTimeStamp() > NewTime)
			{
				break;
			}
			Count++;		
		}
		Joints TempJoint;
		TempJoint.Offset = NewOffset;
		TempJoint.Rot = NewQuat;
		TempJoint.Scale = NewScale;
		if (this->AnimFrames.size() == 0)
		{
			this->AnimFrames.push_back(new Frames(NewTime, TempJoint));
		}
		else
		{
			this->AnimFrames.insert(this->AnimFrames.begin() + Count, 1, new Frames(NewTime, TempJoint));
		}

	}
};
class Animation
{
	std::string Name;
	float TimeLength;
	std::map<std::string, SkelAn*> Skeleton;
	std::vector<std::string> Order;
	InterType CurType;
	glm::mat4 Inv;
public:
	Animation(std::string NewName, std::map<std::string, SkelAn*> Inits,
		std::vector<std::string> NewOrder, float NewTimeLength,glm::mat4 InitInv)
	{
		this->Name = NewName;
		this->Skeleton = Inits;
		this->Order = NewOrder;
		this->TimeLength = NewTimeLength;
		this->Inv = InitInv;
	}
	~Animation()
	{

	}
	float GetTimeLength()
	{
		return this->TimeLength;
	}
	void EditTimeLength(float NewTime)
	{
		this->TimeLength = NewTime;
	}
	std::map<std::string, SkelAn*> GetMap()
	{
		return this->Skeleton;
	}
	std::vector<std::string> GetOrder()
	{
		return this->Order;
	}
	std::string GetAnimName()
	{
		return this->Name;
	}
	glm::mat4 GetCurMat(std::string BoneName,float TimePass)
	{
		return this->Skeleton[BoneName]->GetCurMat(this->Skeleton, TimePass) ;
	}
	glm::mat4 GetMat(std::string BoneName,bool Active)
	{
		this->Skeleton[BoneName]->UpdateRelMat(Active,Skeleton);
		return  this->Skeleton[BoneName]->GetAllMats() ;
	}	
	glm::mat4 GetInv()
	{
		return this->Inv;
	}
};