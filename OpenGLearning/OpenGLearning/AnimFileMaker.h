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
		TRANSMAT,
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
		this->AnimMap["<ModelTransMat>"] = ANIMENUM::TRANSMAT;
		this->AnimMap["<ModelBoneOffsets>"] = ANIMENUM::BONESOFFSET;
		//Animation Information
		this->AnimMap["<AnimName>"] = ANIMENUM::ANIMNAME;
		this->AnimMap["<AnimLength>"] = ANIMENUM::ANIMLENGTH;
		this->AnimMap["<AnimBone>"] = ANIMENUM::BONENAME;
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
		glm::mat4 Temp = glm::mat4(std::stof(Nums[0]),std::stof(Nums[1]), std::stof(Nums[2]), std::stof(Nums[3]),
			std::stof(Nums[4]), std::stof(Nums[5]), std::stof(Nums[6]), std::stof(Nums[7]),
			std::stof(Nums[8]), std::stof(Nums[9]), std::stof(Nums[10]), std::stof(Nums[11]), 
			std::stof(Nums[12]), std::stof(Nums[13]), std::stof(Nums[14]), std::stof(Nums[15]));
		return Temp;
	}
	std::vector<glm::mat4> AllStrToMat(std::vector<std::string> Nums)
	{
		std::vector<glm::mat4> TempMat;
		int Amount = (Nums.size() - 1) / 16;
		std::cout << Amount <<"\n";
		for (int ii = 0; ii < Amount; ii++)
		{
			std::vector<std::string> TempStr;
			for (int jj = 1; jj < 17; jj++)
			{
				TempStr.push_back(Nums[jj + 16 * ii]);
			}
			TempMat.push_back(this->StringToMat4(TempStr));
		}
		return TempMat;
	}
	void SetUpSkelArt(std::vector<std::string> Bones, std::vector<std::string> BoneParent,
		std::vector<SkelArti>& AllInf)
	{
		int TempCount = 0;
		for (auto& jj : Bones)
		{
			SkelArti Temp;			
			Temp.Name = jj;
			Temp.Parent = BoneParent[TempCount];
			Temp.OffsetMat;
			AllInf.push_back(Temp);
			TempCount++;
		}
	}
	//convert strings to Values
	std::vector<glm::vec3> GetAllVec3(std::vector<std::string> Vecs)
	{
		std::vector<glm::vec3> TempVec;
		int Count = Vecs.size();
		for (int ii = 1; ii < Count; ii++)
		{
			TempVec.push_back(this->ToVec3(Vecs[ii]));
		}
		return TempVec;
	}
	std::vector<QuatParts> GetAllQuats(std::vector<std::string> Quats)
	{
		std::vector<QuatParts> TempQuats;
		int Count = Quats.size();
		for (int ii = 1; ii < Count; ii++)
		{
			TempQuats.push_back(this->ToQuat(Quats[ii]));
		}
		return TempQuats;
	}
	std::vector<InterType> GetTypes(std::vector<std::string> Types)
	{
		std::vector<InterType> TempTypes;
		int IntSize = Types.size();
		for (int ii = 1; ii < IntSize; ii++)
		{
			TempTypes.push_back( InterMap[Types[ii]]);
		}
		return TempTypes;
	}
	std::vector<float> GetAllTimes(std::vector <std::string> Vals)
	{
		std::vector<float> TempTimes;
		int TimeSize = Vals.size();
		for (int ii = 1; ii < TimeSize; ii++)
		{
			TempTimes.push_back(std::stof(Vals[ii]));
		}
		return TempTimes;
	}
	std::vector<Frames*> MakeFrames(std::vector<float> Times, std::vector<InterType> Types,
		std::vector<glm::vec3> Pos, std::vector<QuatParts> Quats, std::vector<glm::vec3> Scales)
	{
		std::vector<Frames*> Temp;
		int Count = 0;
		for (auto& ii : Times)
		{
			Joints NewJoints;
			NewJoints.Offset = Pos[Count];
			NewJoints.Rot = Quats[Count];
			NewJoints.Scale = Scales[Count];
			Temp.push_back(new Frames(ii, NewJoints, Types[Count]));
			Count++;
		}
		return Temp;
	}
	//Set AnimArti
	void SetAnimArti(std::vector<SkelArti>& Inits, std::string Bone,
		std::vector<glm::vec3> Pos,std::vector<QuatParts> Quats, std::vector<glm::vec3> Scale,
		std::vector<float> Times,std::vector<InterType> Types,
		std::vector<glm::mat4> TransMat,std::vector<glm::mat4> BoneOffsets)
	{
		std::vector<SkelArti>::iterator it = std::find_if(Inits.begin(), Inits.end(),
			[Bone](const SkelArti& Vi) {return Vi.Name == Bone; });
		int Index = it - Inits.begin();
		//Matrix for Bones and empty channels
		it->TransMat = TransMat[Index];
		it->OffsetMat = BoneOffsets[Index];
		//First Offsets
		if (Pos.size() != 0)
		{
			it->InitOffset = Pos[0];
			it->InitQuat = Quats[0];
			it->InitScale = Scale[0];
		}
		else
		{
			it->InitOffset = glm::vec3(0.f);
			it->InitQuat = QuatParts();
			it->InitScale = glm::vec3(0.f);
		}
		//Frame Creation
		it->AllFrames = this->MakeFrames(Times, Types, Pos, Quats, Scale);
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
		std::vector<std::string> AllBones =	AllAnim[0]->GetOrder();
		int BoneNum = AllBones.size()-1;
		std::string Inv = this->Mat4ToString( AllAnim[0]->GetInv());
		std::string BonesNames = "";
		std::string BonesParents = "";
		std::string OffSetMats = "";
		std::string TransMats = "";
		std::map<std::string, SkelAn*> Skels = AllAnim[0]->GetMap();
		int Count = 0;
		for (auto& ii : AllBones)
		{
			BonesNames += ii + " ";
			BonesParents += Skels[ii]->GetName() + " ";
			if (Count == BoneNum)
			{
				OffSetMats += this->Mat4ToString(Skels[ii]->GetOffsetMat());
				TransMats += this->Mat4ToString(Skels[ii]->GetTransmat());
			}
			else
			{
				OffSetMats += this->Mat4ToString(Skels[ii]->GetOffsetMat(),false);
				TransMats += this->Mat4ToString(Skels[ii]->GetTransmat(),false);
			}
			Count++;
		}
		//Bone offset and InitTransformation
		Make << "<ModelBones> " + BonesNames + "\n";
		Make << "<ModelBonesParents> " + BonesParents + "\n";
		Make << "<ModelInverseTransform> " + Inv;
		Make << "<ModelTransMat> " +  TransMats;
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
			glm::mat4 TempInv;
			SkelArti TempBone;			
			std::vector<std::string> Bones;
			std::vector<std::string> BoneParent;
			std::vector<glm::mat4> TransMats;
			std::vector<glm::mat4> BoneOffsets;
			std::vector<std::string> InvStr;
			//All Vec and TGime And Type
			std::string BoneName = "";
			std::vector<float> BoneTimeInt;
			std::vector<InterType> BoneInterType;
			std::vector<glm::vec3> BonePos;
			std::vector<QuatParts> BoneQuats;
			std::vector<glm::vec3> BoneScale;
			float Frame_TimeStamp;
			int SizeB;
			int SizeID;			
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
					SizeB = out.size();
					for (int ii = 1; ii < SizeB; ii++)
					{
						Bones.push_back(out[ii]);
					}
					break;
				case ANIMENUM::ALLPARID:
					SizeID = out.size();
					for (int ii = 1; ii < SizeID; ii++)
					{
						BoneParent.push_back(out[ii]);
					}
					break;
				case ANIMENUM::INV:
					for ( int ii = 1; ii < 17; ii++)
					{
						InvStr.push_back(out[ii]);
					}
					TempInv = this->StringToMat4(InvStr);
					this->SetUpSkelArt(Bones, BoneParent, TempSkels);
					break;
				case ANIMENUM::BONESOFFSET:
					BoneOffsets = this->AllStrToMat(out);
					break;
				case ANIMENUM::TRANSMAT:
					TransMats = this->AllStrToMat(out);
					break;
				case ANIMENUM::ANIMNAME:
					Temp.Name = out[1];
					break;
				case ANIMENUM::ANIMLENGTH:
					Temp.TimeLength = std::stof(out[1]); 
					Temp.Inv = TempInv;					
					break;
				case ANIMENUM::BONENAME:
					if (BoneName != "")
					{
						//Set AnimBoneFrames
						this->SetAnimArti(TempSkels,BoneName,BonePos,BoneQuats,BoneScale,BoneTimeInt,BoneInterType,TransMats,BoneOffsets);
						//Reset Vecs
						BoneTimeInt.clear();
						BoneInterType.clear();
						BonePos.clear();
						BoneQuats.clear();
						BoneScale.clear();
					}
					BoneName = out[1];
					break;
				case ANIMENUM::ANIMTIME:
					if (out.size() > 1)
					{
						BoneTimeInt = this->GetAllTimes(out);
					}
					break;
				case ANIMENUM::INTERTYPE:
					if (out.size() > 1)
					{
						BoneInterType = this->GetTypes(out);
					}
					break;
				case ANIMENUM::ANIMOFF:
					if (out.size() > 1)
					{
						BonePos = this->GetAllVec3(out);
					}
					break;
				case ANIMENUM::ANIMROT:
					if (out.size() > 1)
					{
						BoneQuats = this->GetAllQuats(out);
					}
					break;
				case ANIMENUM::ANIMSCALE:
					if (out.size() > 1)
					{
						BoneScale = this->GetAllVec3(out);
					}
					break;
				case ANIMENUM::END:
					Temp.Inits = TempSkels;
					DataRead.push_back(Temp);
					BoneName = "";
					std::cout << "END\n";
					break;
				}
			}
			FileData.close();
		}
		return DataRead;
	}
};
