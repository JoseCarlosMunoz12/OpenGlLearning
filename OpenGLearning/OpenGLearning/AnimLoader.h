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