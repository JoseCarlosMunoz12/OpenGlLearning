#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Primitive.h"
class AnimMesh
{
private:
	AnimVertex* AnimVerArray;
	GLuint* IndexArray;
	std::vector<AnimVertex> AnimVerToFind;
	unsigned nrOfVertices;
	unsigned nrOfIndices;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	std::string NameOfMesh;
	void InitVAO()
	{
		//Create VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);
		// Gen VBO and BIND and Send Data
		glGenBuffers(1, &this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(AnimVertex), this->AnimVerArray,GL_STATIC_DRAW);
		//GEN EBO and BIND And Send Data---------
		if (this->nrOfIndices > 0)
		{
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), this->IndexArray, GL_STATIC_DRAW);
		}

	}
};