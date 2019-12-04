#pragma once
#include "Vertex.h"
#include <map>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <vector>
class AnimInf
{
private:
	std::vector<AnimVertex> Vertices;
	std::vector<GLuint> Indices;
	void Set()
	{

	}

public:
	AnimInf()
	{

	}
	~AnimInf()
	{

	}
	inline AnimVertex* GetVertices() { return this->Vertices.data(); };
	inline GLuint* GetIndices() { return this->Indices.data(); };

	inline const unsigned getNrOfVertices() { return this->Vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->Indices.size(); }

};