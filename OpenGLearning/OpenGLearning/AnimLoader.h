 #pragma once
#include "Vertex.h"
#include "SkelAnimat.h"
#include <map>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <sstream>

struct SkelArti
{
	std::string Name;
	std::string Parent;
	std::vector<Frames*> AllFrames;
	glm::vec3 InitOffset;
	glm::vec3 InitScale;
	QuatParts InitQuat;
	glm::mat4 TransMat;
	glm::mat4 OffsetMat;
};
struct AnimArti
{
	float TimeLength;
	std::string Name;
	std::vector<SkelArti> Inits;
	glm::mat4 Inv;
};
//Base Class for Anim Primitives
class AnimInf
{
private:
	std::vector<AnimVertex> Vertices;
	std::vector<GLuint> Indices;
	std::vector<SkelArti> SkelsInits;
	std::vector<AnimArti> AnimInits;
public:
	void set(const AnimVertex* vertices, const unsigned nrofVertices,
		const GLuint* indices, const unsigned nrOfIndices)
	{
		for (size_t i = 0; i < nrofVertices; i++)
		{
			this->Vertices.push_back(vertices[i]);
		}
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->Indices.push_back(indices[i]);
		}
	}
	void set(std::vector<AnimVertex> vertexFound,
		std::vector<GLuint> indicesFound,
		std::vector<AnimArti> SkelsInitsFound)
	{
		this->Vertices = vertexFound;
		this->Indices = indicesFound;
		this->AnimInits = SkelsInitsFound;
	}
	std::vector<AnimArti> Inits()
	{
		return this->AnimInits;
	}
	inline AnimVertex* GetVertices() { return this->Vertices.data(); };
	inline GLuint* GetIndices() { return this->Indices.data(); };
	inline const unsigned getNrOfVertices() { return this->Vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->Indices.size(); }
	
};
//Base classes for the ASSIMP Loading
class Loading
{
private:
	glm::mat4 aiMatToglmMat(aiMatrix4x4 aiVal)
	{
		glm::mat4 glmVal = glm::mat4(aiVal.a1, aiVal.b1, aiVal.c1, aiVal.d1,
			aiVal.a2, aiVal.b2, aiVal.c2, aiVal.d2,
			aiVal.a3, aiVal.b3, aiVal.c3, aiVal.d3,
			aiVal.a4, aiVal.b4, aiVal.c4, aiVal.d4);
		return glmVal;
	}
	glm::vec3 aiVecToglmVec(aiVector3D aiVal)
	{
		return glm::vec3(aiVal.x, aiVal.y, aiVal.z);
	}
protected:
	int Tempsas = 0;
	std::map<std::string, int> BonesId;
	void SetIndex(AnimVertex* Fn, int BoneId, float BoneWieght) 
	{
		if (Fn->MatId.x == -1)
		{
			Fn->MatId.x = BoneId;
			Fn->Weights.x = BoneWieght;
		}
		else if (Fn->MatId.y == -1)
		{
			Fn->MatId.y = BoneId;
			Fn->Weights.y = BoneWieght;

		}
		else if (Fn->MatId.z == -1)
		{
			Fn->MatId.z = BoneId;
			Fn->Weights.z = BoneWieght;
		}
		else if (Fn->MatId.w == -1)
		{
			Fn->MatId.w = BoneId;
			Fn->Weights.w = BoneWieght;
		}
	}
	std::vector<AnimVertex> MakeAnimVertex(aiMesh* meshes)
	{
		std::vector<AnimVertex> Temp;
		for (int ii = 0; ii < meshes->mNumVertices; ii++)
		{
			AnimVertex NewVertex;
			//Position
			NewVertex.position.x = meshes->mVertices[ii].x;
			NewVertex.position.y = meshes->mVertices[ii].y;
			NewVertex.position.z = meshes->mVertices[ii].z;
			//Normals
			NewVertex.normal.x = meshes->mNormals[ii].x;
			NewVertex.normal.y = meshes->mNormals[ii].y;
			NewVertex.normal.z = meshes->mNormals[ii].z;
			//Color
			NewVertex.color = glm::vec3(0.f, 1.f, 0.f);
			//Textures
			NewVertex.texcoord.x = meshes->mTextureCoords[0][ii].x;
			NewVertex.texcoord.y = meshes->mTextureCoords[0][ii].y;
			//MatIds
			NewVertex.MatId.x = -1;
			NewVertex.MatId.y = -1;
			NewVertex.MatId.z = -1;
			NewVertex.MatId.w = -1;
			//matWieghts
			NewVertex.Weights.x = 0.f;
			NewVertex.Weights.y = 0.f;
			NewVertex.Weights.z = 0.f;
			NewVertex.Weights.w = 0.f;
			Temp.push_back(NewVertex);
		}
		return Temp;
	}
	std::vector<GLuint> MakeInd(aiMesh* meshes)
	{
		std::vector<GLuint> FInd;
		for (int ii = 0; ii < meshes->mNumFaces; ii++)
		{
			aiFace Faces = meshes->mFaces[ii];
			FInd.push_back(Faces.mIndices[0]);
			FInd.push_back(Faces.mIndices[1]);
			FInd.push_back(Faces.mIndices[2]);
		}
		return FInd;
	}
	void IndexBones(aiMesh* meshes, std::vector<AnimVertex>& FVert)
	{
		for (int ii = 0; ii < meshes->mNumBones; ii++)
		{
			aiBone* TempBone = meshes->mBones[ii];
			std::string TempName = TempBone->mName.C_Str();
			for (int jj = 0; jj < TempBone->mNumWeights; jj++)
			{
				int Vert = TempBone->mWeights[jj].mVertexId;
				this->SetIndex(&FVert[Vert],this->BonesId[TempName], TempBone->mWeights[jj].mWeight);
			}			
		}
	}
	void CheckForChilds(aiNode* Child, std::string Name,std::vector<SkelArti> &SkelsInit)
	{
		int Temps = Child->mNumChildren;
			
		for (int ii = 0; ii < Temps; ii++)
		{
			SkelArti TemSkel;
			TemSkel.Name = Child->mChildren[ii]->mName.C_Str();
			if (this->BonesId.find(TemSkel.Name) == BonesId.end())
			{
				BonesId[TemSkel.Name] = Tempsas;
				Tempsas++;
			}	
			TemSkel.Parent = Name;			
			SkelsInit.push_back(TemSkel);
			if (Child->mChildren[ii]->mNumChildren != 0)
			{
				this->CheckForChilds(Child->mChildren[ii], Child->mChildren[ii]->mName.C_Str(),SkelsInit);
			}
		}
	}
	void MakeSkelsArt(const aiScene* scene, std::vector<SkelArti> &SkelsInit)
	{
		aiNode* Tem = scene->mRootNode;
		int Amount = Tem->mNumChildren;			
		for (int ii = 0; ii < Amount; ii++)
		{
			int Temps = Tem->mChildren[ii]->mNumChildren;
			if (Temps != 0)
			{
				this->CheckForChilds(Tem->mChildren[ii],"NULL",SkelsInit);
			}
		}
	}
	void SetEachNodes(const aiScene* scene, std::vector<SkelArti> &SkelsInit,glm::mat4& Inv)
	{
		aiMatrix4x4 Set = scene->mRootNode->mTransformation.Inverse();		
		glm::mat4 InvMat = this->aiMatToglmMat(Set);
		Inv = InvMat;
		int Vals = scene->mMeshes[0]->mNumBones;
		for (auto& jj : SkelsInit)
		{
			glm::mat4 OffMat;
			glm::mat4 TransMat;
			aiMatrix4x4 Off;
			for (int ii = 0; ii < Vals; ii++)
			{
				if (scene->mMeshes[0]->mBones[ii]->mName.C_Str() == jj.Name)
				{
					Off = scene->mMeshes[0]->mBones[ii]->mOffsetMatrix;
					break;
				}
			}
			OffMat = this->aiMatToglmMat(Off);
			aiMatrix4x4 Trans = scene->mRootNode->FindNode(jj.Name.c_str())->mTransformation;
			TransMat = this->aiMatToglmMat(Trans);
			aiVector3D TempOffset;
			aiVector3D TempScale;
			aiQuaternion TempQuat;
			Trans.Decompose(TempScale, TempQuat, TempOffset);
			float AngleRad = 2 * glm::acos(TempQuat.w);
			float s = glm::sqrt(1 - TempQuat.w * TempQuat.w);
			glm::vec3 VecQuat;
			AngleRad = AngleRad / glm::pi<float>() * 180.f;
			if (s < 0.001)
			{
				VecQuat = glm::vec3(TempQuat.x, TempQuat.y, TempQuat.z);
			}
			else
			{
				VecQuat = glm::vec3(TempQuat.x / s, TempQuat.y / s, TempQuat.z / s);
				VecQuat = glm::normalize(VecQuat);
			}
			QuatParts TempQuats = QuatParts(AngleRad, VecQuat);
			jj.InitOffset = glm::vec3(TempOffset.x,TempOffset.y,TempOffset.z);
			jj.InitQuat = TempQuats;
			jj.InitScale = glm::vec3(TempScale.x, TempScale.y, TempScale.z);
			jj.TransMat = TransMat;
			jj.OffsetMat = OffMat;
		}		
	}
	void GetAnimFrams(aiAnimation* AnimFound,std::vector<SkelArti> &SkelsInit,std::vector<float> &TimeInit,std::string& AnimName)
	{
		AnimName  = AnimFound->mName.C_Str();
		int AmountOfAnim = AnimFound->mNumChannels;
		TimeInit.push_back(AnimFound->mDuration);
		for (int ii = 0; ii < AmountOfAnim; ii++)
		{
			int NumOfRot = AnimFound->mChannels[ii]->mNumRotationKeys;
			std::string Name = AnimFound->mChannels[ii]->mNodeName.C_Str();			
		
			aiNodeAnim* Temps = AnimFound->mChannels[ii];		
			std::vector<Frames*> TempFrames;
			for (int jj = 0; jj < NumOfRot; jj++)
			{
				float FTime = Temps->mRotationKeys[jj].mTime;
				aiQuaternion TempQuat = Temps->mRotationKeys[jj].mValue;
				aiVector3D Scale = Temps->mScalingKeys[jj].mValue;
				aiVector3D OffSet = Temps->mPositionKeys[jj].mValue;
				float AngleRad = 2 * glm::acos(TempQuat.w);
				float s = glm::sqrt(1 - TempQuat.w * TempQuat.w);
				glm::vec3 VecQuat;
				AngleRad = AngleRad / glm::pi<float>() * 180.f;
				if (s <0.001)
				{
					VecQuat = glm::vec3(TempQuat.x, TempQuat.y, TempQuat.z);
				}
				else
				{
					VecQuat = glm::vec3(TempQuat.x/s, TempQuat.y/s, TempQuat.z/s);
					VecQuat = glm::normalize(VecQuat);
				}			
				QuatParts TempQuats = QuatParts(AngleRad,VecQuat);
				Joints TempJoint;  
				TempJoint.Offset =this->aiVecToglmVec(OffSet);
				TempJoint.Scale = this->aiVecToglmVec(Scale);
				TempJoint.Rot = TempQuats;				
				TempFrames.push_back(new Frames(FTime, TempJoint));
			}
			SkelsInit[this->BonesId[Name]].AllFrames = TempFrames;			
		}
	}
};
class CLoader: public AnimInf,public Loading
{
public:
	CLoader(const char* FileName)
		:AnimInf(),Loading()
	{
		std::vector<AnimVertex> FinalVer;
		std::vector<GLuint> FinalInd;
		std::vector<SkelArti> SkelsInits;
		std::vector<AnimArti> AnimInits;
		std::vector<float> TimeInits;
		std::string File = "Models/ModelCol/";
		glm::mat4 Inv;
		std::string AnimName;
		File += FileName;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
		aiMesh* meshes = scene->mMeshes[0];
		FinalVer = this->MakeAnimVertex(meshes);
		FinalInd =  this->MakeInd(meshes);
		this->MakeSkelsArt(scene,SkelsInits);
		this->SetEachNodes(scene,SkelsInits,Inv);	
		this->IndexBones(meshes,FinalVer);
		AnimInits.push_back({0.f,"",SkelsInits,Inv});
		if (scene->HasAnimations())
		{
			int AnumNum = scene->mNumAnimations;
			for (int ii = 0; ii < AnumNum; ii++)
			{
				this->GetAnimFrams(scene->mAnimations[ii], SkelsInits, TimeInits, AnimName);
				if (TimeInits.size() != 0)
				{
					AnimInits.push_back({ TimeInits[0],AnimName,SkelsInits,Inv });
				}

			}
		}
		this->set(FinalVer, FinalInd,AnimInits);
	}
};
class ClAnimLr : public AnimInf, public Loading
{
public:
	ClAnimLr(std::string ModelName,std::vector<AnimArti> Inits)
		:AnimInf(),Loading()
	{
		std::vector<AnimVertex> FinalVer;
		std::vector<GLuint> FinalInd;
		std::vector<SkelArti> SkelsInits;
		std::vector<AnimArti> AnimInits;
		std::vector<float> TimeInits;
		std::string File = "Models/ModelCol/";
		glm::mat4 Inv;
		File += ModelName;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
		aiMesh* meshes = scene->mMeshes[0];
		FinalVer = this->MakeAnimVertex(meshes);
		FinalInd = this->MakeInd(meshes);
		this->MakeSkelsArt(scene, SkelsInits);
		this->SetEachNodes(scene, SkelsInits,Inv);
		this->IndexBones(meshes, FinalVer);
		AnimInits.push_back({ 0.f,"",SkelsInits,Inv});
		for (auto& jj : Inits)
		{
			AnimInits.push_back(jj);
		}
		this->set(FinalVer, FinalInd, AnimInits);
	}
};
class MulClanimlr : public AnimInf, public Loading
{
public:
	MulClanimlr(std::vector<std::string> FileNames)
		:AnimInf(),Loading()
	{

		std::vector<AnimVertex> FinalVer;
		std::vector<GLuint> FinalInd;
		std::vector<SkelArti> SkelsInits;
		std::vector<AnimArti> AnimInits;
		std::vector<float> TimeInits;
		std::string File = "Models/ModelCol/";
		glm::mat4 Inv;
		std::string AnimName;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(File + FileNames[0], aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
		aiMesh* meshes = scene->mMeshes[0];
		FinalVer = this->MakeAnimVertex(meshes);
		FinalInd = this->MakeInd(meshes);
		this->MakeSkelsArt(scene, SkelsInits);
		this->SetEachNodes(scene, SkelsInits, Inv);
		this->IndexBones(meshes, FinalVer);
		AnimInits.push_back({ 0.f,"",SkelsInits,Inv });
		for (auto& F_Name : FileNames)
		{
			const aiScene* F_scene = importer.ReadFile(File + F_Name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
			if (F_scene->HasAnimations())
			{
				std::stringstream data(F_Name);				
				std::vector<std::string> Anim_Name;
				std::vector<std::string> Anim_Name0;
				std::string Line;
				while (std::getline(data, Line, '_'))
				{
					Anim_Name.push_back(Line);
				}
				std::stringstream Data(Anim_Name[1]);
				while (std::getline(Data, Line, '.'))
				{
					Anim_Name0.push_back(Line);
				}
				this->GetAnimFrams(F_scene->mAnimations[0], SkelsInits, TimeInits, AnimName);
				AnimInits.push_back({ TimeInits[0],Anim_Name0[0],SkelsInits,Inv });
			}
		}
		this->set(FinalVer, FinalInd, AnimInits);
	}
};