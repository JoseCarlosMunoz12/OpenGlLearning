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
		S,
		PARTS
	};
	std::string FolderLoc;
	std::map<std::string, int> AnimMap;
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

		for (auto& ii : AllAnim)
		{
			std::string AnimName = ii->GetAnimName();
			if (AnimName != "")
			{
				float AnimLength = ii->GetTimeLength();
				Make << "<AnimName> " + AnimName + "\n";
				Make << "<AnimLength> " << AnimLength << "\n";
				std::map<std::string,SkelAn*> SkelsInf = ii->GetMap();
				for (auto& jj : SkelsInf)
				{
					Make << "<BoneName> " + jj.first+ "\n";
					std::vector<Frames*> TempFrams = jj.second->GetFrames();
					for (auto& kk : TempFrams)
					{
						Make <<"<s> "<< kk->GetTimeStamp() << "\n";
						Make <<"<Parts> "+ this->ConvertVec(kk->GetOffset()) + " ";
						Make << this->ConvertQuat(kk->GetRot()) + " ";
						Make << this->ConvertVec(kk->GetScale()) << "\n";
					}
				}
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

					break;
				case ANIMENUM::ANIMLENGTH:

					break;
				case ANIMENUM::BONENAME:

					break;
				case ANIMENUM::S:

					break;
				case ANIMENUM::PARTS:

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
