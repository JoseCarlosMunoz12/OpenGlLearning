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
	std::vector<AnimVertex> FinalsVer;
	std::vector<GLuint> FinalInd;
	void MakeAnimVertex()
	{

	}
	void MakeInd()
	{

	}
public:
	ColladaLoader(const char* FileName)
	{
		std::string File = "Models/ModelCol/";
		File += FileName;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	}
	std::vector<AnimVertex> GetVertex()
	{

	}
	std::vector<GLuint> GetIndices()
	{

	}
};

class AnimInf
{
private:
	std::vector<AnimVertex> Vertices;
	std::vector<GLuint> Indices;
	void Set()
	{

	}
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