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
	void set(std::vector<Vertex> Vertices, std::vector<GLuint> indices)
	{
		this->vertices = Vertices;
		this->indices = indices;
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
			glm::vec3( 0.5f, 0.f, 0.5f), glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),  glm::vec3( 0.0f, 0.0f, 1.0f),
			glm::vec3( 0.5f, 0.f,-0.5f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.0f),  glm::vec3( 0.0f, 0.0f, 1.0f),
			glm::vec3(-0.5f, 0.f,-0.5f), glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.0f),  glm::vec3( 0.0f, 0.0f, 1.0f),
			glm::vec3(-0.5f, 0.f, 0.5f), glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(1.0f,1.0f),  glm::vec3( 0.0f, 0.0f, 1.0f)
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			2,0,1,  //Triangle 2
			2,3,0 //Trianlge 1
			
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class CustomTerrain :Primitive
{
	CustomTerrain(int Size,const int NumVertex)
		:Primitive()
	{
		
		std::vector<Vertex> vertex;
		std::vector<GLuint> indices;
		for (size_t ii =0; ii < NumVertex; ii++)
		{

		}
		this->set(vertex, indices);

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

			7, 6, 1,
			7, 1, 0,

			4, 5, 6,
			4, 6, 7,

			3, 2, 5,
			3, 5, 4
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};