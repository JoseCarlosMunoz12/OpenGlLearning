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
		ALLBONES,
		ALLPARID,
		INV,
		BONESOFFSET,
		BONENAME,
		BONEPARENT,
		INTERTYPE,
		ANIMTIME,
		ANIMOFF,
		ANIMROT,
		ANIMSCALE,
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
		//File information Parser
		this->AnimMap["<ModelName>"] = ANIMENUM::MODNAME;
		//Skeleton Information
		this->AnimMap["<ModelBones>"] = ANIMENUM::ALLBONES;
		this->AnimMap["<ModelBonesParents>"] = ANIMENUM::ALLPARID;
		this->AnimMap["<ModelInverseTransform>"] = ANIMENUM::INV;
		this->AnimMap["<ModelBoneOffsets>"] = ANIMENUM::BONESOFFSET;
		//Animation Information
		this->AnimMap["<AnimName>"] = ANIMENUM::ANIMNAME;
		this->AnimMap["<AnimLength>"] = ANIMENUM::ANIMLENGTH;
		this->AnimMap["<AnimTimeInt>"] = ANIMENUM::ANIMTIME;
		this->AnimMap["<AnimType>"] = ANIMENUM::INTERTYPE;
		this->AnimMap["<AnimOffset>"] = ANIMENUM::ANIMOFF;
		this->AnimMap["<AnimRotate>"] = ANIMENUM::ANIMROT;
		this->AnimMap["<AnimScale>"] = ANIMENUM::ANIMSCALE;
		//End of catching Animation informaiton
		this->AnimMap["<END>"] = ANIMENUM::END;
		//Enum to to string and vice versa
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
	glm::mat4 StringToMat4(std::vector<std::string> Nums)
	{

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
		std::string BonesParents = "";
		std::string OffSetMats = "";
		std::map<std::string, SkelAn*> Skels = AllAnim[0]->GetMap();
		int Count = 0;
		for (auto& ii : AllBones)
		{
			BonesNames += ii + " ";
			BonesParents += Skels[ii]->GetName() + " ";
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
		Make << "<ModelBonesParents> " + BonesParents + "\n";
		Make << "<ModelInverseTransform> " + Inv;
		Make << "<ModelBoneOffsets> " + OffSetMats;
		//Animation information
		for (auto& ii : AllAnim)		{
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
					Make << "<AnimBone> " + jj + "\n";
					std::string AnimInt = "";
					std::string AnimType = "";
					std::string AnimOffset = "";
					std::string AnimRot = "";
					std::string AnimScale = "";
					std::vector<Frames*> TempFrams = SkelsInf[jj]->GetFrames();
					for (auto& kk : TempFrams)
					{
						AnimType += this->ChosenInter[kk->GetType()] + " ";
						AnimInt += std::to_string(kk->GetTimeStamp()) + " ";
						AnimOffset += this->ConvertVec(kk->GetOffset()) + " ";
						AnimRot += this->ConvertQuat(kk->GetRot()) + " ";
						AnimScale += this->ConvertVec(kk->GetScale()) + " ";
					}
					Make << "<AnimTimeInt> " + AnimInt + "\n";
					Make << "<AnimType> " + AnimType + "\n";
					Make << "<AnimOffset> " + AnimOffset + "\n";
					Make << "<AnimRotate> " + AnimRot + "\n";
					Make << "<AnimScale> " + AnimScale + "\n";
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
					std::cout << out[1] + "\n";
					break;
				case ANIMENUM::ALLBONES:
					std::cout << out[1] + "\n";
					break;
				case ANIMENUM::ALLPARID:
					std::cout << out[1] + "\n";
					break;
				case ANIMENUM::INV:
					std::cout << out[1] + "\n";
					break;
				case ANIMENUM::BONESOFFSET:
					std::cout << out[1] + "\n";
					break;
				case ANIMENUM::ANIMNAME:
					std::cout << out[1] + "\n";
					break;
				case ANIMENUM::ANIMLENGTH:
					std::cout << out[1] + "\n"; 
					break;
				case ANIMENUM::ANIMTIME:
					if (out.size() > 1)
					{
						std::cout << out[1] + "\n";
					}
					break;
				case ANIMENUM::INTERTYPE:
					if (out.size() > 1)
					{
						std::cout << out[1] + "\n";
					}
					break;
				case ANIMENUM::ANIMOFF:
					if (out.size() > 1)
					{
						std::cout << out[1] + "\n";
					}
					break;
				case ANIMENUM::ANIMROT:
					if (out.size() > 1)
					{
						std::cout << out[1] + "\n";
					}
					break;
				case ANIMENUM::ANIMSCALE:
					if (out.size() > 1)
					{
						std::cout << out[1] + "\n";
					}
					break;
				case ANIMENUM::END:
					std::cout << "END\n";
					break;
				}
			}
			FileData.close();
		}
		return DataRead;
	}
};
