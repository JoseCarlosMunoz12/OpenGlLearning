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
public:
	AnimFileRdrMkr(std::string FolderLoc)
		:FolderLoc(FolderLoc)
	{

	}
	~AnimFileRdrMkr()
	{

	}
	void WriteFile(std::vector<Animation*> AllAnim, std::string FileName)
	{
		std::ofstream Make;
		Make.open(this->FolderLoc + FileName + ".txt");

		Make.close();
	}
};
