#pragma once
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include "Vertex.h"

class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

public:
	Primitive()
	{

	}
	virtual ~Primitive()
	{

	}
	void set(const Vertex* vertices, const unsigned nrofVertices,
		const GLuint* indices,const unsigned nrOfIndices)
	{
		for (size_t i = 0; i < nrofVertices; i++)
		{
			this->vertices.push_back(vertices[i]);
		}
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indices.push_back(indices[i]);
		}
	}
	void set(std::vector<Vertex> vertexFound, std::vector<GLuint> indicesFound)
	{
		this->vertices = vertexFound;
		this->indices = indicesFound;
	}
	inline Vertex* getVertices() { return this->vertices.data();}
	inline GLuint* getIndices() { return this->indices.data();}

	inline const unsigned getNrOfVertices() { return this->vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->indices.size(); }

};

class Quad : public Primitive
{
public:
	Quad()
		:Primitive()
	{
		Vertex vertices[] =
		{
			//Position                    //Color                     //TexCoords			 //Normals
			glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),  glm::vec3(0.0f,0.0f, 1.0f),
			glm::vec3(-0.5f,-0.5f, 0.f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.0f),  glm::vec3(0.0f,0.0f, 1.0f),
			glm::vec3(0.5f,-0.5f, 0.f), glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.0f),  glm::vec3(0.0f,0.0f, 1.0f),
			glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(1.0f,1.0f),  glm::vec3(0.0f,0.0f, 1.0f)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0,1,2, //Trianlge 1
			0,2,3  //Triangle 2
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class PlaneTerrain : public Primitive
{
public:
	PlaneTerrain()
		:Primitive()
	{
		Vertex vertices[] = 
		{
			//Position                    //Color                     //TexCoords			 //Normals
			glm::vec3(-0.5f, 0.f, 0.5f), glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),  glm::vec3( 0.0f, 1.0f, 0.0f),
			glm::vec3(-0.5f, 0.f,-0.5f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.0f),  glm::vec3( 0.0f, 1.0f, 0.0f),
			glm::vec3( 0.5f, 0.f, 0.5f), glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,1.0f),  glm::vec3( 0.0f, 1.0f, 0.0f),
			glm::vec3( 0.5f, 0.f,-0.5f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(1.0f,0.0f),  glm::vec3( 0.0f, 1.0f, 0.0f)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			1,0,2,  //Triangle 1
			1,2,3,  //Trianlge 2
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class CustomTerrain :public Primitive
{
public:
	CustomTerrain()
		:Primitive()
	{
		Vertex v[9 * 9];
		GLuint ind[9 * 9 * 3 * 2];
		glm::vec3 positions;
		glm::vec3 colors = glm::vec3(1.f,1.f,1.f);
		glm::vec2 texCoords;
		glm::vec3 normals;
		int vertexPointer = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j <9; j++) {
				//Position
				positions.y = 0;
				positions.x = (float)j / ((float)9-1 ) * 8;
				positions.z = (float)i / ((float)9-1 ) * 8;
				//normals
				normals.x = 0;
				normals.y = 11;
				normals.z = 0;
				//Tex Coords
				texCoords.x = (float)j / ((float)9-1);
				texCoords.y = (float)i / ((float)9-1);

				v[vertexPointer]= {positions,colors,texCoords,normals};
				if (i * j < 9*9)
					vertexPointer++;
			}
		}
		unsigned nrOfVertices = sizeof(v) / sizeof(Vertex);
		int pointer = 0;
		for (GLuint gz = 0; gz < 9 - 1; gz++) {
			for (GLuint gx = 0; gx < 9 - 1; gx++) {
				GLuint topLeft = (gz*9) + gx;
				GLuint topRight = topLeft + 1;
				GLuint bottomLeft = ((gz+1)*9) + gx;
				GLuint bottomRight = bottomLeft + 1;
				//Indices
				ind[pointer++] = topLeft;
				ind[pointer++] = bottomLeft;
				ind[pointer++] = topRight;
				ind[pointer++] = topRight;
				ind[pointer++] = bottomLeft;
				ind[pointer++] = bottomRight;
			}
		}
		unsigned nrOfIndices = sizeof(ind) / sizeof(GLuint);
		
		this->set(v,nrOfVertices,ind,nrOfIndices);
	}
	CustomTerrain(int Size, int Dimension)
	{
		std::vector<Vertex> VertexOfTerrain;
		std::vector<GLuint> IndecesOfTerrain;
		glm::vec3 positions;
		glm::vec3 colors = glm::vec3(1.f, 1.f, 1.f);
		glm::vec2 texCoords;
		glm::vec3 normals;
		for (int jj = 0;jj < Dimension;jj++)
		{
			for (int ii = 0; ii < Dimension; ii++)
			{
				positions.y = 0;
				positions.x = (float)ii / ((float)Dimension - 1) * Size;
				positions.z = (float)jj / ((float)Dimension - 1) * Size;
				normals.x = 0;
				normals.y = 11;
				normals.z = 0;
				texCoords.x = (float)ii / ((float)Dimension - 1);
				texCoords.y = (float)jj / ((float)Dimension - 1);
				Vertex TempVertex = {positions,colors,texCoords,normals};
				VertexOfTerrain.push_back(TempVertex);
			}
		}
		for (int jj = 0; jj < Dimension - 1; jj++)
		{
			for (int ii = 0; ii < Dimension - 1; ii++)
			{
				GLuint TopLeft = (jj * Dimension) + ii;
				GLuint TopRight = TopLeft + 1;
				GLuint BottomLeft = ((jj + 1) * Dimension) + ii;
				GLuint BottomRight = BottomLeft + 1;
				IndecesOfTerrain.push_back(TopLeft);
				IndecesOfTerrain.push_back(BottomLeft);
				IndecesOfTerrain.push_back(TopRight);
				IndecesOfTerrain.push_back(TopRight);
				IndecesOfTerrain.push_back(BottomLeft);
				IndecesOfTerrain.push_back(BottomRight);
			}
		}
		this->set(VertexOfTerrain, IndecesOfTerrain);
	}
};

class Triangle : public Primitive
{
public:
	Triangle()
		:Primitive()
	{
		Vertex vertices[] =
		{
			//Position                    //Color                     //TexCoords			 //Normals
			glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),  glm::vec3(0.0f,0.0f, 1.0f),
			glm::vec3(-0.5f,-0.5f, 0.f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.0f),  glm::vec3(0.0f,0.0f, 1.0f),
			glm::vec3(0.5f,-0.5f, 0.f), glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.0f),  glm::vec3(0.0f,0.0f, 1.0f),
			glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(1.0f,1.0f),  glm::vec3(0.0f,0.0f, 1.0f)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0,1,2, //Trianlge 1
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class Pyramid : public Primitive
{
public:
	Pyramid()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position								//Color							//Texcoords					//Normals
			//Triangle front
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),

			//Triangle left
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(-1.f, 0.f, 0.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(-1.f, 0.f, 0.f),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(-1.f, 0.f, 0.f),

			//Triangle back
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),

			//Triangles right
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		this->set(vertices, nrOfVertices, nullptr, 0);
	}
};

class Cube : public Primitive
{
public:
	Cube()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position								//Color							//Texcoords					//Normals
			glm::vec3(-0.5f, 0.5f, 0.5f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.5f),			glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),

			glm::vec3(0.5f, 0.5f, -0.5f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, 0.5f, -0.5f),			glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f,  1.f)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2,
			0, 2, 3,

			7, 2, 1,
			7, 1, 0,

			4, 5, 2,
			4, 2, 7,

			3, 2, 5,
			3, 5, 4
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};