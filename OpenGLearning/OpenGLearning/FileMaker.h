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
		Temp += std::to_string(VecToString.z);
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
			Make << "Model_Name@" + Name;
			Make << "\n";
			Make << "--Material ID--\n";
			int MatId = ii->GetShaderId();
			Make << "M_Mat_ID@" + std::to_string(MatId) + "\n";
			Make << "--Meshes--\n";
			int MeshCount = 0;
			for (auto& jj : Meshess)
			{
				Make<<"M_M@"<< std::to_string(MeshCount) + "@" + jj->GiveName() + "\n";				
				MeshCount++;
			}
			Make << "--Textures--\n";
			std::vector<GeneralTextInfo*> ModTex = ii->getTexture();
			int TexCount = 0;
			for (auto& jj : ModTex)
			{
				Make << "M_T@" << std::to_string(TexCount) << "@" << jj->GiveChar() << "\n";
				TexCount++;
			}
			std::vector<std::vector<int>> TexId = ii->GetTexId();
			int TexIdCount = 0;
			for (auto& jj : TexId)
			{
				Make << "Node_TexID@" + std::to_string(TexIdCount) + "@";
				for (auto& kk : jj)
				{
					Make << std::to_string(kk) + "-";
				}
				Make << "\n";
				TexIdCount++;
			}			
			Make << "--Model Position--\n";			
			Make << "M_P@" + this->TransposeVec3(ii->GetPosition()) << "\n";
			Make << "--Model Rotation--\n";
			Make << "M_R@" + this->TransposeVec3(ii->GetRotation(0)) << "\n";
			Make << "--Model Scale--\n";
			Make << "M_S@" + this->TransposeVec3(ii->GetScale(0)) << "\n";
			Make << "--Model Nodes Information--\n";
			std::vector<Nodes*> ModNodes = ii->GetNodesInfo();
			int NodeCount = 0;
			for (auto& jj : ModNodes)
			{
				Make << "M_N_Inf@" + std::to_string(NodeCount) << "\n";
				Make << "M_N_ID@" + jj->GetNodesId() + "\n";
				Make << "M_N_M_ID@" + std::to_string(jj->GetMeshId()) << "\n";
				Make << "M_N_P@Node@" + std::to_string(NodeCount)<< "@" + this->TransposeVec3(jj->GetPosition()) + "\n";
				Make << "M_N_R@Node@" + std::to_string(NodeCount) + "@" + this->TransposeVec3(jj->GetRotation()) << "\n";
				Make << "M_N_S@Node@" + std::to_string(NodeCount) + "@" + this->TransposeVec3(jj->GetScale()) << "\n";
				Make << "*----*" <<"\n";
				NodeCount++;
			}
			Make << "++@\n";
			Count++;
		}
		Make.close();
	}
};

class FileReader 
{
private:
	static enum StringVal
	{
		Model_Name = 1,
		M_MAT,
		M_M,
		M_T,
		N_ID,
		M_P,
		M_R,
		M_S,
		M_N_Inf,
		M_N_M_ID,
		M_N_ID,
		M_N_P,
		M_N_R,
		M_N_S,
		PLUS
	};
	std::string FileLoc;
	std::map< std::string, StringVal > StringFound;
	//Funcs to Find Special Char
	void InitMap()
	{
		this->StringFound["Model_Name"] = Model_Name;
		this->StringFound["M_Mat_ID"] = M_MAT;
		this->StringFound["M_M"] = M_M;
		this->StringFound["M_T"] = M_T;
		this->StringFound["Node_TexID"] = N_ID;
		this->StringFound["M_P"] = M_P;
		this->StringFound["M_R"] = M_R;
		this->StringFound["M_S"] = M_S;
		this->StringFound["M_N_Inf"] = M_N_Inf;
		this->StringFound["M_N_M_ID"] = M_N_M_ID;
		this->StringFound["M_N_ID"] = M_N_ID;
		this->StringFound["M_N_P"] = M_N_P;
		this->StringFound["M_N_R"] = M_N_R;
		this->StringFound["M_N_S"] = M_N_S;
		this->StringFound["++"] = PLUS;
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
	//Functions To TranslateData
public:
	FileReader(std::string NewFileLoc)
	{
		this->FileLoc = NewFileLoc;
		this->InitMap();
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
				if (Line.find("@") != std::string::npos || Line.find("+")!= std::string::npos)
				{
					std::vector< std::string> out;
					this->ReturnStringArray(Line,'@',out);					
					switch (this->StringFound[out[0]])
					{
					case Model_Name:
						Lines += out[1] + "\n";
						break;
					case M_MAT:
						Lines += out[1] + "\n";
						break;
					case M_M:
						Lines += out[1] + "-" + out[2] + "\n";
						break;
					case M_T:
						Lines += out[1] + "-" + out[2] + "\n";
						break;
					case N_ID:
						Lines += out[2] + "\n";
						break;
					case M_P:
						Lines += out[1] + "\n";
						break;
					case M_R:
						Lines += out[1] + "\n";
						break;
					case M_S:
						Lines += out[1] + "\n";
						break;
					case M_N_Inf:
						Lines += out[1] + "\n";
						break;
					case M_N_ID:
						Lines += out[1] + "\n";
						break;
					case M_N_P:
						Lines += out[3] + "\n";
						break;
					case M_N_R:
						Lines += out[3] + "\n";
						break;
					case M_N_S:
						Lines += out[3] + "\n";
						break;
					case PLUS:
						Lines += out[0] + "\n";
						break;
					default:
						break;
					}
				}
			}
			FileData.close();
		}
		return Lines;
	}
	std::vector<M_To_Make> DecipherFile()
	{
		std::vector<M_To_Make> DataRead;
		M_To_Make TempData;
		std::fstream FileData(this->FileLoc);
		if (FileData.is_open())
		{
			std::string Line;
			while (std::getline(FileData, Line))
			{
				if (Line.find("@") != std::string::npos || Line.find("+") != std::string::npos)
				{
					std::vector<std::string> out;
					std::vector<std::string> NewOut;
					this->ReturnStringArray(Line, '@', out);
					switch (this->StringFound[out[0]])
					{
					case Model_Name:
						TempData.NewName = out[1];
						break;
					case M_MAT:
						TempData.MatId = std::atoi(out[1].c_str());
						break;
					case M_M:
						TempData.MeshesName.push_back(out[2]);
						break;
					case M_T:
						TempData.TexNames.push_back(out[2]);
						break;
					case N_ID:
						this->ReturnStringArray(out[2], '-', NewOut);
						break;
					case M_P:
						break;
					case M_R:
						break;
					case M_S:
						break;
					case M_N_Inf:
						break;
					case M_N_ID:
						break;
					case M_N_M_ID:
						break;
					case M_N_P:
						break;
					case M_N_R:
						break;
					case M_N_S:
						break;
					case PLUS:
						DataRead.push_back(TempData);
						TempData.MeshesName.clear();
						TempData.TexNames.clear();
						TempData.MeshId.clear();
						TempData.NodesInf.clear();
						TempData.TexId.clear();
						break;
					default:
						break;
					}
				}
			}
			FileData.close();
		}
		return DataRead;
	}	
};