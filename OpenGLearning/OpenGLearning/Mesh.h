#pragma once

#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 ModelMatrix;
	void InitVertexData(Vertex* VertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices)
	{
		for (size_t i = 0; i < nrOfVertices; i++)
			{
			this->vertices.push_back(VertexArray[i]);
			}
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indices.push_back(indexArray[i]);
		}
	}

	void InitVAO()
	{
		glCreateVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VBO AND BIND AND SEND DATA

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		//GEN EBO and BIND And Send Data---------

		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

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
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}
	void updateUniforms(Shader* shader)
	{
		shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
	}
public:
	Mesh(Vertex* VertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices)
	{
		this->InitVertexData(VertexArray,nrOfVertices,indexArray,nrOfIndices);
		this->InitVAO();
		this->InitModelMatrix();
		
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}
	void Update()
	{

	}

	void render(Shader* shader)
	{
		//Update Uniforms
		this->updateUniforms(shader);
		shader->use();
		//BInd VAO
		glBindVertexArray(this->VAO);
		//Draw
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}

};