#pragma once
#include "libs.h"
#include <fstream>
#include <iostream>
class FileMaker 
{
private:
	std::string FileLoc;
public:
	FileMaker(std::string MakeFile)
	{
		this->FileLoc = MakeFile;
	}
	void WriteFile(std::vector<Model*> AllModels)
	{
		std::ofstream Make;
		Make.open(this->FileLoc);
		int Count = 0;
		for (auto& ii : AllModels)
		{
			std::string Name = ii->GetName();
			std::vector<Mesh*> Meshess = ii->GetMeshes();
			Make << "*******Model " +std::to_string(Count) +"*******\n";
			Make << Name.c_str();
			Make << "\n";
			Make << "--Meshes--\n";
			int MeshCount = 0;
			for (auto& jj : Meshess)
			{
				Make<<"M_N-";
				Make << jj->GiveName();
				Make << " " + std::to_string(MeshCount)+ "\n";				
				MeshCount++;
			}
			Make << "--Model Position--\n";
			glm::vec3 ModPos = ii->GetPosition(); 
			
			Make << "M_P-";
			Make << std::to_string(ModPos.x) << "-" << std::to_string(ModPos.y) << "-" << std::to_string(ModPos.z) << "-\n";
			Make << "--Model Nodes--\n";
			std::vector<Nodes*> ModNodes = ii->GetNodesInfo();


			Count++;
		}
		Make.close();
	}
	void MakeGameFile(Game* GameToget)
	{
		std::cout << "Found\n";
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