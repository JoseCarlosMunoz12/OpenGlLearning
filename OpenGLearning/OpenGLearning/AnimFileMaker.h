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
	std::string FolderLoc;
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
public:
	AnimFileRdrMkr(std::string FolderLoc)
		:FolderLoc(FolderLoc)
	{

	}
	~AnimFileRdrMkr()
	{

	}
	void WriteFile(std::vector<Animation*> AllAnim,
		std::string AnimModel,std::string FileName)
	{
		std::ofstream Make;
		Make.open(this->FolderLoc + FileName + ".txt");
		Make <<"--------<" + AnimModel + ">--------\n";

		for (auto& ii : AllAnim)
		{
			std::string AnimName = ii->GetAnimName();
			float AnimLength = ii->GetTimeLength();
			Make << "--------<< " + AnimName + " >>--------\n";
			Make << "<< " << AnimLength << " >>\n";
			std::map<std::string,SkelAn*> SkelsInf = ii->GetMap();
			for (auto& jj : SkelsInf)
			{
				Make << "<<< " + jj.first+ " >>>\n";
				std::vector<Frames*> TempFrams = jj.second->GetFrames();
				for (auto& kk : TempFrams)
				{
					Make << kk->GetTimeStamp() << "\n";
					Make << this->ConvertVec(kk->GetOffset()) + " ";
					Make << this->ConvertQuat(kk->GetRot()) + " ";
					Make << this->ConvertVec(kk->GetScale()) << "\n";
				}
			}
		}
		Make.close();
	}
	std::map<std::string, Animation*> ReadFile()
	{

	}
};
