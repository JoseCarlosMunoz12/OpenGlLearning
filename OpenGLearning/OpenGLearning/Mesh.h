#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Primitive.h"

class Mesh 
{
private:
	Vertex* vertexArray;
	GLuint* indexArray;
	std::vector<Vertex> VertexTofind;
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
		
		//GEN VBO AND BIND AND SEND DATA

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

		//GEN EBO and BIND And Send Data---------
		if (this->nrOfIndices > 0)
		{
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices* sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
		}

		//Set VerttexAttribPointers and Enable (input assembly)
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//TexCoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		//Normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);
		//BIND VAO 0
		glBindVertexArray(0);
	}
	void updateUniforms(glm::mat4 FinalMatrix,Shader* shader)
	{
		shader->setMat4fv(FinalMatrix, "ModelMatrix");
	}
public:
	Mesh(std::unique_ptr<Primitive> primitive,
		std::string Name)
	{
		this->NameOfMesh = Name;

		this->nrOfIndices = primitive->getNrOfIndices();
		this->nrOfVertices = primitive->getNrOfVertices();
		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < this->nrOfVertices; i++)
		{
			this->vertexArray[i] = primitive->getVertices()[i];
			VertexTofind.push_back(vertexArray[i]);
		}
		this->indexArray = new GLuint[this->nrOfIndices];
		for (size_t i = 0; i <this-> nrOfIndices; i++)
		{
			this->indexArray[i] = primitive->getIndices()[i];
		}

		this->InitVAO();
	}
	Mesh(Vertex* VertexArray,
		std::string Name,
		const unsigned& nrOfVertices, GLuint* indexArray,
		const unsigned& nrOfIndices)
	{
		this->NameOfMesh = Name;

		this->nrOfIndices = nrOfIndices;
		this->nrOfVertices = nrOfVertices;
		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertexArray[i] = VertexArray[i];
			VertexTofind.push_back(vertexArray[i]);
		}
		this->indexArray = new GLuint[this->nrOfIndices];
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indexArray[i] = indexArray[i];
		}
		this->InitVAO();
	}
	Mesh(const Mesh& obj)
	{
		this->NameOfMesh = obj.NameOfMesh;
		this->nrOfIndices = obj.nrOfIndices;
		this->nrOfVertices = obj.nrOfVertices;
		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertexArray[i] = obj.vertexArray[i];
			VertexTofind.push_back(vertexArray[i]);
		}
		this->indexArray = new GLuint[this->nrOfIndices];
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indexArray[i] = obj.indexArray[i];
		}
		this->InitVAO();
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		if (this->nrOfIndices > 0)
		{
			glDeleteBuffers(1, &this->EBO);
		}
		delete[] this->vertexArray;
		delete[] this->indexArray;
	}
	//Accessors
	void Render(glm::mat4 FinalMatrix,Shader* shader)
	{
		//Update Uniforms
		this->updateUniforms(FinalMatrix,shader);
		shader->use();
		//BInd VAO
		glBindVertexArray(this->VAO);
		//Render
		if (this->nrOfIndices == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
		}
		//Clean up
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	const char* GiveName()
	{
		return this->NameOfMesh.c_str();
	}

};