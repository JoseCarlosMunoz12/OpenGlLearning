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

class ColladaLoader
{
	std::vector<AnimVertex> FinalVer;
	std::vector<GLuint> FinalInd;
	std::vector<SkelArti> SkelsInits;
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
			float AllVals = Fn->Weights.x + Fn->Weights.y + Fn->Weights.z;
			Fn->Weights.x /= AllVals;
			Fn->Weights.y /= AllVals;
			Fn->Weights.z /= AllVals;
		}

	}
	void MakeAnimVertex(aiMesh* meshes)
	{
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
			//matWieghts
			NewVertex.Weights.x = 0.f;
			NewVertex.Weights.y = 0.f;
			NewVertex.Weights.z = 0.f;
			this->FinalVer.push_back(NewVertex);
		}
	}
	void MakeInd(aiMesh* meshes)
	{
		for (int ii = 0; ii < meshes->mNumFaces; ii++)
		{
			aiFace Faces = meshes->mFaces[ii];
			FinalInd.push_back(Faces.mIndices[0]);
			FinalInd.push_back(Faces.mIndices[1]);
			FinalInd.push_back(Faces.mIndices[2]);
		}
	}
	void IndexBones(aiMesh* meshes)
	{
		for (int ii = 0; ii < meshes->mNumBones; ii++)
		{
			aiBone* TempBone = meshes->mBones[ii];
			for (int jj = 0; jj < TempBone->mNumWeights; jj++)
			{
				int Vert = TempBone->mWeights[jj].mVertexId;
				this->SetIndex(&this->FinalVer[Vert],ii, TempBone->mWeights[jj].mWeight);
			}			
		}

	}
	void CheckForChilds(aiNode* Child, std::string Name)
	{
		int Temps = Child->mNumChildren;
		for (int ii = 0; ii < Temps; ii++)
		{
			
			SkelArti TemSkel;
			TemSkel.Name = Child->mChildren[ii]->mName.C_Str();
			TemSkel.Parent = Name;			
			this->SkelsInits.push_back(TemSkel);
			if (Child->mChildren[ii]->mNumChildren != 0)
			{
				this->CheckForChilds(Child->mChildren[ii], Child->mChildren[ii]->mName.C_Str());
			}
		}
		

	}
	void MakeSkelsArt(const aiScene* scene)
	{
		aiNode* Tem = scene->mRootNode;
		int Amount = Tem->mNumChildren;
		for (int ii = 0; ii < Amount; ii++)
		{
			int Temps = Tem->mChildren[ii]->mNumChildren;
			if (Temps !=0)
			{
				this->CheckForChilds(Tem->mChildren[ii],"NULL");
			}
		}
	}
	aiMatrix4x4 GetParentMatrix(const aiScene* scene, std::string Name, std::string Parent)
	{
		if (Parent == "NULL")
		{
			return scene->mRootNode->FindNode(Name.c_str())->mTransformation;
		}else {
			for (auto& jj : SkelsInits)
			{
				if (jj.Name == Parent)
				{
					return GetParentMatrix(scene,jj.Name,jj.Parent) * scene->mRootNode->FindNode(Name.c_str())->mTransformation;
					break;
				}
			}
		}
	}
	void SetEachNodes(const aiScene* scene)
	{	
		for (auto& jj : SkelsInits)
		{
			aiMatrix4x4 TempMat = this->GetParentMatrix(scene, jj.Name, jj.Parent);
			aiVector3D TempOffset;
			aiVector3D TempScale;
			aiQuaternion TempQuat;
			TempMat.Decompose(TempScale, TempQuat, TempOffset);			
			jj.InitOffset = glm::vec3(TempOffset.x,TempOffset.z,-TempOffset.y);
			jj.InitQuat = QuatParts();
			jj.InitScale = glm::vec3(TempScale.x, TempScale.z, TempScale.y);
		}		
	}
	aiMatrix4x4 GetAnimMatr(const aiScene* scene, std::string Name)
	{
		for (auto& jj : SkelsInits)
		{
			if (jj.Name == Name)
			{
				return this->GetParentMatrix(scene, jj.Name, jj.Parent);
			}
		}
	}

		
	void GetEachAnimFrams(const aiScene* scene)
	{		
		if (scene->HasAnimations())
		{
			int NumOfAnim = scene->mNumAnimations;
			for (int ii = 0; ii < NumOfAnim; ii++)
			{
				aiAnimation* TempAnim = scene->mAnimations[ii];
				int NumChans = TempAnim->mNumChannels;

				for (int jj = 0; jj < NumChans; jj++)
				{
					
					std::string name = TempAnim->mChannels[jj]->mNodeName.C_Str();					
				}
				

			}
		}
	}

public:
	ColladaLoader(const char* FileName)
	{
		std::string File = "Models/ModelCol/";
		File += FileName;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |aiProcess_FlipUVs);
		aiMesh* meshes = scene->mMeshes[0];
		this->MakeAnimVertex(meshes);
		this->MakeInd(meshes);
		this->IndexBones(meshes);
		this->MakeSkelsArt(scene);
		this->SetEachNodes(scene);
	}
	std::vector<AnimVertex> GetVertex()
	{
		return this->FinalVer;
	}
	std::vector<GLuint> GetIndex()
	{
		return this->FinalInd;
	}
	std::vector<SkelArti> Inits()
	{
		return this->SkelsInits;
	}
};

class AnimInf : public ColladaLoader
{
private:
	std::vector<AnimVertex> Vertices;
	std::vector<GLuint> Indices;
	std::vector<Frames*> AllFrames;
public:
	AnimInf(const char* FileName)
		:ColladaLoader(FileName)
	{		
		this->Vertices = this->GetVertex();
		this->Indices = this->GetIndex();
	}
	~AnimInf()
	{

	}
	inline AnimVertex* GetVertices() { return this->Vertices.data(); };
	inline GLuint* GetIndices() { return this->Indices.data(); };

	inline const unsigned getNrOfVertices() { return this->Vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->Indices.size(); }
};