#pragma once
#include "Vertex.h"
#include "SkelAnimat.h"
#include <map>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>

class ColladaLoader
{
private:

public:
	ColladaLoader(const char* FileName)
	{
		std::ifstream File;
		File.open(FileName);

		if (File.is_open())
		{
			bool Found = false;
			std::string zTemp;
			int Count = 0;
			while (std::getline(File,zTemp))
			{
				if (Found or ( zTemp.find("<library_geometries>" )!= std::string::npos))
				{					
					Found = true;
					std::cout << std::to_string(Count) + "-found\n";
					Count++;


				}
				if (Found)
				{
					if (zTemp.find("</library_geometries>") != std::string::npos)
					{
						std::cout << "exited \n";
						break;
					}
				}

			}

		}
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