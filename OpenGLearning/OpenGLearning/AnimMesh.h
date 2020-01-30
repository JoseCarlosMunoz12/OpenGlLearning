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
	std::vector<AnimArti> Inits;
	std::vector<float> TimeInits;
	std::string NameOfMesh;
	void InitVAO()
	{
		//Create VAO
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);
		// Gen VBO and BIND and Send Data
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
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
		glVertexAttribIPointer(4, 4, GL_INT, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, MatId));
		glEnableVertexAttribArray(4);
		//Weights
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, Weights));
		glEnableVertexAttribArray(5);
		//BIND VAO 0
		glBindVertexArray(0);
	}
	void UpdateUniforms(glm::mat4 FinalMatrix,Shader* shader)
	{
		shader->setMat4fv(FinalMatrix, "ModelMatrix");
	}
	void UpdateMatInf(std::vector<glm::mat4> AllMats, Shader* shader)
	{
		if (AllMats.size() != 0)
		{
		int TempCount = 0;
		shader->set1i(AllMats.size(), "TransCount");
		for (auto&	CurMat : AllMats)
			{
				std::string MatName = "Bones[" + std::to_string(TempCount)+"]";
				shader->setMat4fv(CurMat,MatName.c_str());
				TempCount++;
			}
		}
		else
		{
			for (size_t ii = 0; ii < 100; ii++)
			{
				std::string MatName = "Bones[" + std::to_string(ii) + "]";
				shader->setMat4fv(glm::mat4(1.f), MatName.c_str());
			}
		}
	}
public:
	AnimMesh(AnimInf* NewMesh, std::string NewName)
	{
		this->NameOfMesh = NewName;
		this->nrOfIndices = NewMesh->getNrOfIndices();
		this->nrOfVertices = NewMesh->getNrOfVertices();
		this->AnimVerArray = new AnimVertex[this->nrOfVertices];
		for (size_t ii = 0; ii < this->nrOfVertices; ii++)
		{
			this->AnimVerArray[ii] = NewMesh->GetVertices()[ii];
			AnimVerToFind.push_back(AnimVerArray[ii]);
		}
		this->IndexArray = new GLuint[this->nrOfIndices];
		for (size_t ii = 0; ii < this->nrOfIndices; ii++)
		{
			this->IndexArray[ii] = NewMesh->GetIndices()[ii];
		}
		this->Inits = NewMesh->Inits();
		this->InitVAO();
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
	void Render(glm::mat4 FinalMatrix, Shader* shader,std::vector<glm::mat4> AllMats)
	{
		//UpdateUniforms
		this->UpdateMatInf(AllMats, shader);
		this->UpdateUniforms(FinalMatrix, shader); 
		shader->use();
		//Bind VAO
		glBindVertexArray(this->VAO);
		if (this->nrOfIndices == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
		}
		else {
			glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
		}
		//Clean up
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	std::string GiveName()
	{
		return this->NameOfMesh;
	}
	std::vector<AnimArti> GetInits()
	{
		return this->Inits;
	}
	std::vector<float> GetTimes()
	{
		return this->TimeInits;
	}
};