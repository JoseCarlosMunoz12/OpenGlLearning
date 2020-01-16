#pragma once
#include "Vertex.h"
#include "SkelAnimat.h"
#include <map>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct SkelArti
{
	std::string Name;
	std::string Parent;
	std::vector<Frames*> AllFrames;
	glm::vec3 InitOffset;
	glm::vec3 InitScale;
	QuatParts InitQuat;
};

class AnimInf
{
private:
	std::vector<AnimVertex> Vertices;
	std::vector<GLuint> Indices;
	std::vector<SkelArti> SkelsInits;
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
	void set(std::vector<AnimVertex> vertexFound, std::vector<GLuint> indicesFound, std::vector<SkelArti> SkelsInitsFound)
	{
		this->Vertices = vertexFound;
		this->Indices = indicesFound;
		this->SkelsInits = SkelsInitsFound;
	}
	std::vector<SkelArti> Inits()
	{
		return this->SkelsInits;
	}
	inline AnimVertex* GetVertices() { return this->Vertices.data(); };
	inline GLuint* GetIndices() { return this->Indices.data(); };
	inline const unsigned getNrOfVertices() { return this->Vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->Indices.size(); }
	
};

class CLoader: public AnimInf
{
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
			NewVertex.position.y = meshes->mVertices[ii].z;
			NewVertex.position.z = -1 * meshes->mVertices[ii].y;
			//Normals
			NewVertex.normal.x = meshes->mNormals[ii].x;
			NewVertex.normal.y = meshes->mNormals[ii].z;
			NewVertex.normal.z = meshes->mNormals[ii].y;
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
			if (Temps !=0)
			{
				this->CheckForChilds(Tem->mChildren[ii],"NULL",SkelsInit);
			}
		}
	}
	aiMatrix4x4 GetParentMatrix(const aiScene* scene, std::string Name, std::string Parent,std::vector<SkelArti> &SkelsInit)
	{
		if (Parent == "NULL")
		{
			return scene->mRootNode->FindNode(Name.c_str())->mTransformation;
		}else {
			for (auto& jj : SkelsInit)
			{
				if (jj.Name == Parent)
				{
					return GetParentMatrix(scene,jj.Name,jj.Parent,SkelsInit) * scene->mRootNode->FindNode(Name.c_str())->mTransformation;
					break;
				}
			}
		}
	}
	void SetEachNodes(const aiScene* scene, std::vector<SkelArti> &SkelsInit)
	{	
		for (auto& jj : SkelsInit)
		{
			aiMatrix4x4 TempMat = this->GetParentMatrix(scene, jj.Name, jj.Parent,SkelsInit);
			aiVector3D TempOffset;
			aiVector3D TempScale;
			aiQuaternion TempQuat;
			TempMat.Decompose(TempScale, TempQuat, TempOffset);			
			jj.InitOffset = glm::vec3(TempOffset.x,TempOffset.z,-TempOffset.y);
			jj.InitQuat = QuatParts();
			jj.InitScale = glm::vec3(TempScale.x, TempScale.z, TempScale.y);
		}		
	}
	aiMatrix4x4 GetAnimMatr(const aiScene* scene, std::string Name, std::vector<SkelArti> &SkelsInit)
	{
		for (auto& jj : SkelsInit)
		{
			if (jj.Name == Name)
			{
				return this->GetParentMatrix(scene, jj.Name, jj.Parent,SkelsInit);
			}
		}
	}
	void GetAnimFrams(const aiScene* scene,std::vector<SkelArti> SkelsInit)
	{
		if (!scene->HasAnimations())
		{
			std::cout << "None\n";
			return;
		}
		aiAnimation* AnimFound = scene->mAnimations[0];
		int AmountOfAnim = AnimFound->mNumChannels;
		for (int ii = 0; ii < AmountOfAnim; ii++)
		{
			std::string Name = AnimFound->mChannels[ii]->mNodeName.C_Str();
			std::cout << Name << "\n";
			std::cout << SkelsInit[this->BonesId[Name]].Name << "\n";
			std::cout << AnimFound->mChannels[ii]->mNumPositionKeys << "\n";
			std::cout << AnimFound->mChannels[ii]->mNumRotationKeys << "\n";
			std::cout << AnimFound->mChannels[ii]->mNumScalingKeys << "\n";
		}
	}
public:
	CLoader(const char* FileName)
		:AnimInf()
	{
		std::vector<AnimVertex> FinalVer;
		std::vector<GLuint> FinalInd;
		std::vector<SkelArti> SkelsInits;
		std::string File = "Models/ModelCol/";
		File += FileName;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);
		aiMesh* meshes = scene->mMeshes[0];
		FinalVer = this->MakeAnimVertex(meshes);
		FinalInd =  this->MakeInd(meshes);
		this->MakeSkelsArt(scene,SkelsInits);
		this->SetEachNodes(scene,SkelsInits);	
		this->IndexBones(meshes,FinalVer);	
		this->GetAnimFrams(scene,SkelsInits);
		this->set(FinalVer, FinalInd,SkelsInits);
	}
};
