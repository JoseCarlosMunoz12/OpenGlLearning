#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Primitive.h"
#include "Collision.h"

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
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 origin;
	glm::mat4 ModelMatrix;
	Collision MeshCollisionBox;

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

	void InitModelMatrix()
	{
		this->position = glm::vec3(0.f);
		this->rotation = glm::vec3(0.f);
		this->scale = glm::vec3(1.f);
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position- this->origin);
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);

	}
	void updateUniforms(Shader* shader)
	{
		shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
	}
	void updateModelMatrix()
	{

		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}
public:
	Mesh(Primitive* primitive,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 origin = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.f))
	{
		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;
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
		this->updateModelMatrix();
		this->MeshCollisionBox.CreateCollisionBox(VertexTofind);
		this->MeshCollisionBox.ShowPos();
	}

	Mesh(Vertex* VertexArray, 
		const unsigned& nrOfVertices, GLuint* indexArray,
		const unsigned& nrOfIndices,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 origin = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.f))
	{
		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale =scale;

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
		this->updateModelMatrix();
		this->MeshCollisionBox.CreateCollisionBox(VertexTofind);
	}

	Mesh(const Mesh& obj)
	{
		this->position = obj.position;
		this->origin = obj.origin;
		this->rotation = obj.rotation;
		this->scale = obj.scale;

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
		this->updateModelMatrix();
		this->MeshCollisionBox.CreateCollisionBox(VertexTofind);
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

	//Modifieres
	void setPosition(const glm::vec3 position)
	{
		this->position = position;
	}
	void setOrigin(const glm::vec3 origin)
	{
		this->origin = origin;
	}
	void setRotation(const glm::vec3 rotation)
	{
		this->rotation = rotation;
	}
	void setScale(const glm::vec3 setScale)
	{
		this->scale = setScale;
	}
	//Functions
	void move(const glm::vec3 position)
	{
		this->position += position;
		this->MeshCollisionBox.UpdateBoxPos(position);
	}
	void rotate(const glm::vec3 rotation)
	{
		this->rotation += rotation;
	}
	void scaleUp(const glm::vec3 rescale)
	{
		this->scale += rescale;
	}

	void Update(glm::vec3 RayPos)
	{
		this->MeshCollisionBox.CheckForCollision(RayPos);
	}

	glm::mat4 ReturnModelMatrix()
	{
		return this->ModelMatrix;
	}
	void render(Shader* shader)
	{
		//Update Uniforms
		this->updateModelMatrix();
		this->updateUniforms(shader);
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
	
};