#pragma once
#include "libs.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <map>
namespace fs = std::filesystem;

class AnimFileRdrMkr
{
	static enum ANIMENUM
	{
		MODNAME,
		ANIMNAME,
		ANIMLENGTH,
		BONENAME,
		BONEPARENT,
		INTERTYPE,
		S,
		PARTS,
		END
	};
	std::string FolderLoc;
	std::map<std::string, int> AnimMap;
	std::map<InterType, std::string> ChosenInter;
	std::map<std::string, InterType> InterMap;
	std::string ConvertVec(glm::vec3 Vec)
	{
		std::string Temp = "";
		Temp = std::to_string(Vec.x) + "*";
		Temp += std::to_string(Vec.y) + "*";
		Temp += std::to_string(Vec.z) ;
		return Temp;
	}
	std::string ConvertQuat(QuatParts Quat)
	{
		std::string Temp = "";
		Temp = std::to_string(Quat.Angle) + "*";
		Temp += ConvertVec(Quat.UnitVec);
		return Temp;
	}
	glm::vec3 ToVec3(std::string Info)
	{
		std::vector<std::string> out;
		this->ReturnStringArray(Info, '*', out);
		return glm::vec3(std::stof(out[0]),
			std::stof(out[1]), 
			std::stof(out[2]));
	}
	QuatParts ToQuat(std::string Info)
	{
		std::vector<std::string> out;
		this->ReturnStringArray(Info, '*', out);
		return QuatParts(std::stof(out[0]),
			glm::vec3(std::stof(out[1]),
				std::stof(out[2]),
				std::stof(out[3])));
	}
	Joints MakeJoints(std::vector<std::string> Info)
	{
		Joints TempJoint;		
		TempJoint.Offset = this->ToVec3(Info[0]);
		TempJoint.Rot = this->ToQuat(Info[1]);
		TempJoint.Scale = this->ToVec3(Info[2]);
		return TempJoint;
	}
	Frames* GetFrames(float TimeStamp, std::vector<std::string> Info,InterType Type)
	{
		return new Frames(TimeStamp,this->MakeJoints(Info),Type);		
	}
	void ReturnStringArray(std::string const& str, const char delim, std::vector<std::string>& out)
	{
		size_t start;
		size_t end = 0;
		while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
		{
			end = str.find(delim, start);
			out.push_back(str.substr(start, end - start));
		}
	}
	void InitMap()
	{
		this->AnimMap["<ModelName>"] = ANIMENUM::MODNAME;
		this->AnimMap["<AnimName>"] = ANIMENUM::ANIMNAME;
		this->AnimMap["<AnimLength>"] = ANIMENUM::ANIMLENGTH;
		this->AnimMap["<BoneName>"] = ANIMENUM::BONENAME;
		this->AnimMap["<s>"] = ANIMENUM::S;
		this->AnimMap["<Parts>"] = ANIMENUM::PARTS;
		this->AnimMap["<END>"] = ANIMENUM::END;
		this->AnimMap["<BoneParent>"] = ANIMENUM::BONEPARENT;
		this->AnimMap["<InterType>"] = ANIMENUM::INTERTYPE;
		this->ChosenInter[HOLD] = "HOLD";
		this->ChosenInter[LINEAR] = "LINEAR";
		this->ChosenInter[QUADBENZ] = "QUADBENZ";
		this->ChosenInter[CUBEBENZ] = "CUBEBENZ";
		for (auto& jj : ChosenInter)
		{
			this->InterMap[jj.second] = jj.first;
		}
	}
	std::string Mat4ToString(glm::mat4 Mat,bool CrL = true)
	{
		std::string Temp = "";
		for (int ii = 0; ii < 4; ii++)
		{
			for (int jj = 0; jj < 4; jj++)
			{
				Temp += std::to_string(Mat[ii][jj]);
				if (jj == 3 and ii == 3 and CrL)
				{
					Temp += "\n";
				}
				else
				{
					Temp += " ";
				}
			}
		}
		return Temp;
	}
public:
	AnimFileRdrMkr(std::string FolderLoc)
		:FolderLoc(FolderLoc)
	{
		this->InitMap();
	}
	~AnimFileRdrMkr()
	{

	}
	void WriteFile(std::vector<Animation*> AllAnim,
		std::string AnimModel,std::string FileName)
	{
		std::ofstream Make;
		Make.open(this->FolderLoc + FileName + ".txt");
		Make <<"<ModelName> " + AnimModel + "\n";	
		std::vector<std::string> AllBones =	AllAnim[0]->GetOrder();
		int BoneNum = AllBones.size()-1;
		std::string Inv = this->Mat4ToString( AllAnim[0]->GetInv());
		std::string BonesNames = "";
		std::string OffSetMats = "";
		std::map<std::string, SkelAn*> Skels = AllAnim[0]->GetMap();
		int Count = 0;
		for (auto& ii : AllBones)
		{
			BonesNames += ii + " ";
			if (Count == BoneNum)
			{
				OffSetMats += this->Mat4ToString(Skels[ii]->GetOffsetMat());
			}
			else
			{
				OffSetMats += this->Mat4ToString(Skels[ii]->GetOffsetMat(),false);
			}
			Count++;
		}
		//Bone offset and InitTransformation
		Make << "<ModelBones> " + BonesNames + "\n";
		Make << "<ModelInversTransform> " + Inv;
		Make << "<ModelTransforms>\n";
		Make << "<ModelBoneOffsets> " + OffSetMats;
		//Animation information
		for (auto& ii : AllAnim)
		{
			std::string AnimName = ii->GetAnimName();
			if (AnimName != "")
			{
				float AnimLength = ii->GetTimeLength();
				Make << "<AnimName> " + AnimName + "\n";
				Make << "<AnimLength> " << AnimLength << "\n";
				std::vector<std::string> Order = ii->GetOrder();
				std::map<std::string,SkelAn*> SkelsInf = ii->GetMap();
				for (auto& jj : Order)
				{
					Make << "<BoneName> " + jj + "\n";
					Make << "<BoneParent> " + SkelsInf[jj]->GetName() + "\n";
					std::vector<Frames*> TempFrams = SkelsInf[jj]->GetFrames();
					for (auto& kk : TempFrams)
					{
						Make << "<InterType> " << this->ChosenInter[kk->GetType()] <<"\n";
						Make <<"<s> "<< kk->GetTimeStamp() << "\n";
						Make <<"<Parts> "+ this->ConvertVec(kk->GetOffset()) + " ";
						Make << this->ConvertQuat(kk->GetRot()) + " ";
						Make << this->ConvertVec(kk->GetScale()) << "\n";
					}
				}
				Make << "<END>\n";
			}
		}
		Make.close();
	}
	std::vector<AnimArti> ReadFile(std::string FileName)
	{
		std::vector<AnimArti> DataRead;
		std::fstream FileData(this->FolderLoc + FileName);
		if (FileData.is_open())
		{
			AnimArti Temp;
			std::vector<SkelArti> TempSkels;
			SkelArti TempBone;
			TempBone.InitOffset = glm::vec3(1.f);
			TempBone.InitScale = glm::vec3(1.f);
			InterType Type = InterType::HOLD;
			float Frame_TimeStamp;
			std::string Line;
			while (std::getline(FileData, Line))
			{
				std::vector<std::string> out;
				this->ReturnStringArray(Line, ' ', out);
				switch (this->AnimMap[out[0]])
				{
				case ANIMENUM::MODNAME:
					break;
				case ANIMENUM::ANIMNAME:
					Temp.Name = out[1];
					break;
				case ANIMENUM::ANIMLENGTH:
					Temp.TimeLength = std::stof(out[1]);
					break;
				case ANIMENUM::BONENAME:
					if (TempBone.Name != "")
					{
						Temp.Inits.push_back(TempBone);
						TempBone.AllFrames.clear();
					}
					TempBone.Name = out[1];	
					break;
				case ANIMENUM::BONEPARENT:
					TempBone.Parent = out[1];
					break;
				case ANIMENUM::INTERTYPE:
					Type = this->InterMap[out[1]];
					break;
				case ANIMENUM::S:
					Frame_TimeStamp = std::stof(out[1]);
					break;
				case ANIMENUM::PARTS:
					TempBone.AllFrames.push_back(
						this->GetFrames(Frame_TimeStamp,
										{out[1],out[2],out[3]},Type));
					TempBone.InitOffset = TempBone.AllFrames[0]->GetOffset();					
					break;
				case ANIMENUM::END:	
					Temp.Inits.push_back(TempBone);
					DataRead.push_back(Temp);					
					break;
				default:
					break;
				}
			}
			FileData.close();
		}
		return DataRead;
	}
};
