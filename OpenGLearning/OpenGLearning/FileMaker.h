#pragma once
#include "libs.h"
#include <fstream>
#include <iostream>
class FileMaker 
{
private:
	std::string FileLoc;
	std::string TransposeVec3(glm::vec3 VecToString)
	{
		std::string Temp = "";
		Temp += std::to_string(VecToString.x) + "-";
		Temp += std::to_string(VecToString.y) + "-";
		Temp += std::to_string(VecToString.z) + "-";
		return Temp;
	}
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
			Make << "Model Name is " + Name;
			Make << "\n";
			Make << "--Meshes--\n";
			int MeshCount = 0;
			for (auto& jj : Meshess)
			{
				Make<<"M_M-"<< std::to_string(MeshCount) + jj->GiveName() + "\n";				
				MeshCount++;
			}
			Make << "--Textures\n";
			std::vector<GeneralTextInfo*> ModTex = ii->getTexture();
			int TexCount = 0;
			for (auto& jj : ModTex)
			{
				Make << "M_T-" << std::to_string(TexCount) << "-" << jj->GiveChar() << "\n";
				TexCount++;
			}
			Make << "--Model Position--\n";			
			Make << "M_P-" + this->TransposeVec3(ii->GetPosition()) << "-\n";
			Make << "--Model Nodes Information--\n";
			std::vector<Nodes*> ModNodes = ii->GetNodesInfo();
			int NodeCount = 0;
			for (auto& jj : ModNodes)
			{
				Make << "M_N_P-Node_ "<< std::to_string(NodeCount)<< "-" <<this->TransposeVec3(jj->GetPosition()) << "\n";
				NodeCount++;
			}
			
			Count++;
		}
		Make.close();
	}
};

class FileReader 
{
private:

	std::string FileLoc;
public:
	FileReader(std::string NewFileLoc)
	{
		this->FileLoc = NewFileLoc;
	}

	std::string GetFileInfo()
	{
		std::fstream FileData(this->FileLoc);
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