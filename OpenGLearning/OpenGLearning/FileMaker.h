#pragma once
#include "libs.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <map>
namespace fs = std::filesystem;
class FileRdrMkr
{
private:
	//File Reader Funcs
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
		M_N_O,
		PLUS
	};
	std::vector<std::string> Files;
	std::string FolderLoc;
	std::map<std::string, StringVal > StringFound;
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
		this->StringFound["M_N_O"] = M_N_O;
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
	//Find Name in Vector
	bool FindItem(std::string Item)
	{
		std::vector<std::string>::iterator it;
		it = std::find(Files.begin(), Files.end(), Item);
		if (it != Files.end())
		{
			return true;
		}
		return false;
	}
	//Functions To TranslateData
	glm::vec3 ToVec3(std::string Line)
	{
		glm::vec3 TempVec;
		std::vector<std::string> TempString;
		this->ReturnStringArray(Line, '$', TempString);
		TempVec.x = std::stoi(TempString[0]);
		TempVec.y = std::stoi(TempString[1]);
		TempVec.z = std::stoi(TempString[2]);
		return TempVec;
	}
	std::vector<int> ToVecIn(std::string Line)
	{
		std::vector<int> TempVec;
		std::vector<std::string> TempString;
		this->ReturnStringArray(Line, '-', TempString);
		for (auto& ii : TempString)
		{
			TempVec.push_back(std::stoi(ii));
		}
		return TempVec;
	}
	//File Maker Funcs
	std::string TransposeVec3(glm::vec3 VecToString)
	{
		std::string Temp = "";
		Temp += std::to_string(VecToString.x) + "$";
		Temp += std::to_string(VecToString.y) + "$";
		Temp += std::to_string(VecToString.z);
		return Temp;
	}
public:
	FileRdrMkr(std::string FolderLoc)
		:FolderLoc(FolderLoc)
	{
		this->InitMap();
	}
	//Get Data
	std::vector<MdlToMake> DecipherFile(int FileId)
	{
		std::vector<MdlToMake> DataRead;
		MdlToMake TempData;
		NodesId TempNodes;
		std::fstream FileData(this->FolderLoc + this->Files[FileId]);
		if (FileData.is_open())
		{
			std::string Line;
			while (std::getline(FileData, Line))
			{
				if (Line.find("@") != std::string::npos || Line.find("+") != std::string::npos)
				{
					std::vector<std::string> out;
					std::vector<std::string> NewOut;
					std::vector<int> TempIn;
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
						TempNodes.TexId = this->ToVecIn(out[2]);
						break;
					case M_P:
						TempData.NewPos = this->ToVec3(out[1]);
						break;
					case M_R:
						TempData.NewRot = this->ToVec3(out[1]);
						break;
					case M_S:
						TempData.NewScale = this->ToVec3(out[1]);
						break;
					case M_N_Inf:
						break;
					case M_N_ID:
						TempNodes.ParentId = std::stoi(out[1]);
						break;
					case M_N_M_ID:
						TempNodes.MeshId = std::stoi(out[1]);
						break;
					case M_N_P:
						TempNodes.Pos = this->ToVec3(out[3]);
						break;
					case M_N_R:
					//	TempNodes.Rot = this->ToVec3(out[3]);
						break;
					case M_N_S:
						TempNodes.Scale = this->ToVec3(out[3]);
						break;
					case M_N_O:
						TempNodes.Origin = this->ToVec3(out[3]);
						TempData.NodesInf.push_back(TempNodes);
						TempNodes.TexId.clear();
						break;
					case PLUS:
						DataRead.push_back(TempData);
						TempData.MeshesName.clear();
						TempData.TexNames.clear();
						TempData.NodesInf.clear();
						TempIn.clear();
						TempNodes.TexId.clear();
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
	//Write Data
	void WriteFile(std::vector<Model*> AllModels,std::string FileName)
	{
		std::ofstream Make;
		Make.open(this->FolderLoc + FileName + ".txt");
		int Count = 0;
		for (auto& ii : AllModels)
		{
			std::string Name = ii->GetName();
			std::vector<Mesh*> Meshess = ii->GetMeshes();
			Make << "*******Model " + std::to_string(Count) + "*******\n";
			Make << "Model_Name@" + Name;
			Make << "\n";
			Make << "--Material ID--\n";
			int MatId = ii->GetStdMat()->GetMatId();
			Make << "M_Mat_ID@" + std::to_string(MatId) + "\n";
			Make << "--Meshes--\n";
			int MeshCount = 0;
			for (auto& jj : Meshess)
			{
				Make << "M_M@" << std::to_string(MeshCount) + "@" + jj->GiveName() + "\n";
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
			Make << "--Model Position--\n";
			Make << "M_P@" + this->TransposeVec3(ii->GetPosition()) << "\n";
			Make << "--Model Rotation--\n";
	//		Make << "M_R@" + this->TransposeVec3(ii->GetRotation()) << "\n";
			Make << "--Model Scale--\n";
			Make << "M_S@" + this->TransposeVec3(ii->GetScale()) << "\n";
			Make << "--Model Origin--\n";
			Make << "M_O@" + this->TransposeVec3(ii->GetOrigin()) << "\n";
			Make << "--Model Nodes Information--\n";
			std::vector<Nodes*> ModNodes = ii->GetNodesInfo();
			int NodeCount = 0;
			for (auto& jj : ModNodes)
			{
				Make << "M_N_Inf@" + std::to_string(NodeCount) << "\n";
				Make << "M_N_ID@" + jj->GetParentsId() + "\n";
				Make << "M_N_M_ID@" + std::to_string(jj->GetMeshId()) << "\n";
				Make << "Node_TexID@" + std::to_string(NodeCount) + "@";
				for (auto& kk : TexId[NodeCount])
				{
					Make << std::to_string(kk) + "-";
				}
				Make << "\n";
				Make << "M_N_P@Node@" + std::to_string(NodeCount) << "@" + this->TransposeVec3(jj->GetPosition()) + "\n";
	//			Make << "M_N_R@Node@" + std::to_string(NodeCount) + "@" + this->TransposeVec3(jj->GetRotation()) << "\n";
				Make << "M_N_S@Node@" + std::to_string(NodeCount) + "@" + this->TransposeVec3(jj->GetScale()) << "\n";
				Make << "M_N_O@Node@" + std::to_string(NodeCount) + "@" + this->TransposeVec3(jj->GetOrigin()) << "\n";
				Make << "*----*" << "\n";
				NodeCount++;
			}
			Make << "++@\n";
			Count++;
		}
		Make.close();
	}
	void GetFilesFolder()
	{
		for (const auto& entry : fs::directory_iterator(this->FolderLoc))
		{			
			std::vector<std::string> PathDiv;			
			std::string TempString = entry.path().string();
			this->ReturnStringArray(TempString, '/', PathDiv);
			if (!this->FindItem(PathDiv[2]))
			{
				this->Files.push_back(PathDiv[2]);
			}
		}
	}
	std::string VerifyName(std::vector<Model*> AllModels,std::string NewFileName)
	{
		if (this->FindItem(NewFileName + ".txt"))
		{
			return "File Exist, choose Different Name";
		}
		else{
			this->WriteFile(AllModels, NewFileName);
			this->GetFilesFolder();
			return "File Added";
		}
	}
	std::vector<std::string> GetAllFiles()
	{
		return this->Files;
	}
};