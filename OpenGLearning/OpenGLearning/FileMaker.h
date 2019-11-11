#pragma once
#include "libs.h"
#include <fstream>
#include <iostream>
class FileMaker 
{
private:

public:
	FileMaker()
	{
	}
	void MakeFiles(std::string FileLoc)
	{
		std::ofstream Name(FileLoc);
		Name << std::to_string(4568) + "\n";
		Name << std::to_string(4568) + "\n";
		Name << std::to_string(4568) + "\n";
	}
};

class FileReader 
{
private:

public:
	FileReader()
	{
	}

	std::string GetFileInfo(std::string FileLoc)
	{
		std::fstream FileData(FileLoc);
		std::string Line;
		std::string Lines = "";
		if (FileData.is_open())
		{
			while (std::getline(FileData, Line))
			{
				Lines += Line + "\n";
			}
			FileData.close();
		}
		return Lines;
	}
};