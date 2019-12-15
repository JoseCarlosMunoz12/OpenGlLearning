#pragma once
#include "Vertex.h"
#include "SkelAnimat.h"
#include <map>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class ColladaLoader
{
private:
	std::vector<AnimVertex> FinalVer;
	std::vector<GLuint> FinalInd;
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
public:
	ColladaLoader(const char* FileName)
	{
		std::string File = "Models/ModelCol/";
		File += FileName;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		aiMesh* meshes = scene->mMeshes[0];
		this->MakeAnimVertex(meshes);
		this->MakeInd(meshes);
		this->IndexBones(meshes);
	}
	std::vector<AnimVertex> GetVertex()
	{
		return this->FinalVer;
	}
	std::vector<GLuint> GetIndices()
	{
		return this->FinalInd;
	}
};

class AnimInf
{
private:
	std::vector<AnimVertex> Vertices;
	std::vector<GLuint> Indices;
public:
	AnimInf(const char* FileName)
	{
		std::ifstream in_file;
		std::string Temps = "Models/ModelCol/";
		Temps += FileName;
		ColladaLoader Reader(Temps.c_str());
		this->Vertices = Reader.GetVertex();
		this->Indices = Reader.GetIndices();
	}
	~AnimInf()
	{

	}
	inline AnimVertex* GetVertices() { return this->Vertices.data(); };
	inline GLuint* GetIndices() { return this->Indices.data(); };

	inline const unsigned getNrOfVertices() { return this->Vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->Indices.size(); }

};