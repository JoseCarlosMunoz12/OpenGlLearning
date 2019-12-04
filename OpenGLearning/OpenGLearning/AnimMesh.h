#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "AnimLoader.h"

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
		//Set Vertex AtrivPoitsers an enable (input assembly)
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, color));
		glEnableVertexAttribArray(1);
		//TexCoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, normal));
		glEnableVertexAttribArray(3);
		//MatIds
		glVertexAttribIPointer(4, 3, GL_INT, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, MatId));
		glEnableVertexAttribArray(4);
		//Weights
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, Weights));
		glEnableVertexAttribArray(3);
		//BIND VAO 0
		glBindVertexArray(0);
	}
public:
	AnimMesh(AnimInf* NewMesh, std::string NewName)
	{
		this->NameOfMesh = NewName;
	}
	~AnimMesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		if (this->nrOfIndices > 0)
		{
			glDeleteBuffers(1, &this->EBO);
		}
		delete[] this->AnimVerArray;
		delete[] this->IndexArray;
	}
};