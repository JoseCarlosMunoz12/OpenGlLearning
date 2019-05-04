#pragma once
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <string>

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
				positions.x = (((float)j-4.5f) / ((float)9-1 )) * 1;
				positions.z = (((float)i-4.5f) / ((float)9-1 )) * 1;
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
		glm::vec3 colors = glm::vec3(1.f, 0.f, 1.f);
		glm::vec2 texCoords;
		glm::vec3 normals;
		for (int jj = 0;jj < Dimension;jj++)
		{
			for (int ii = 0; ii < Dimension; ii++)
			{
				positions.y = 0;
				positions.x = ((float)ii - Dimension/2)/ ((float)Dimension - 1) * Size;
				positions.z = ((float)jj - Dimension / 2) / ((float)Dimension - 1) * Size;
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
		:Primitive()
	{
		std::vector<Vertex> VertexOfCube;
		std::vector<GLuint> IndecesOfCube;
		glm::vec3 Normals[] =
		{
			glm::vec3(0.f,0.f,1.f), glm::vec3(-1.f,0.f,0.f),
			glm::vec3(0.f,0.f,-1.f),glm::vec3(1.f,0.f,0.f),
		};
		glm::vec2 TexCoords[] = 
		{
			glm::vec2(1.f,1.f),glm::vec2(0.f,1.f),
			glm::vec2(0.f,0.f),glm::vec2(1.f,0.f)
		};
		glm::vec3 ColorsUsed = glm::vec3(0.f, 0.f, 1.f);
		glm::vec3 Positions[] = 
		{
			//Top Plane
			glm::vec3(0.5f,0.5f,0.5f),glm::vec3(-0.5f,0.5f,0.5f),
			glm::vec3(-0.5f,0.5f,-0.5f),glm::vec3(0.5f,0.5f,-0.5f),
			//Bottom Plane
			glm::vec3(0.5f,-0.5f,0.5f),glm::vec3(-0.5f,-0.5f,0.5f),
			glm::vec3(-0.5f,-0.5f,-0.5f),glm::vec3(0.5f,-0.5f,-0.5f)
		};
		int Part0 = 0;
		int Part1 = 1;
		int Part2 = 5;
		int Part3 = 4;
		for (size_t ii = 1; ii < 5; ii++)
		{
		
			Vertex Vertex0 = {Positions[Part0],ColorsUsed,TexCoords[0],Normals[ii-1]};
			Vertex Vertex1 = {Positions[Part1],ColorsUsed,TexCoords[1],Normals[ii-1]};
			Vertex Vertex2 = {Positions[Part2],ColorsUsed,TexCoords[2],Normals[ii-1]};
			Vertex Vertex3 = {Positions[Part3],ColorsUsed,TexCoords[3],Normals[ii-1]};
			
			VertexOfCube.push_back(Vertex1);
			VertexOfCube.push_back(Vertex2);
			VertexOfCube.push_back(Vertex0);
			VertexOfCube.push_back(Vertex0);
			VertexOfCube.push_back(Vertex2);
			VertexOfCube.push_back(Vertex3);
			
			if (Part1 == 3)
			{
				Part1 = 0;
			}
			else
			{
				Part1 += 1;
			}
			if (Part2 == 7)
			{
				Part2 = 4;
			}
			else
			{
				Part2 += 1;
			}
			Part0 += 1;
			Part3 += 1;
		}
			   
		for (size_t ii = 0; ii < VertexOfCube.size(); ii++)
		{
			IndecesOfCube.push_back(ii);
		}

		this->set(VertexOfCube, IndecesOfCube);
	}

	Cube(int Dimension, float DimensionSize)
		:Primitive()
	{
		std::vector<Vertex> VertexOfCube;
		std::vector<GLuint> IndecesOfCube;
		glm::vec3 Colors = glm::vec3(0.f, 1.f, 1.f);
		glm::vec3 Normals[] =
		{
			glm::vec3(0.f,0.f,1.f), glm::vec3(-1.f,0.f,0.f),
			glm::vec3(0.f,0.f,-1.f),glm::vec3(1.f,0.f,0.f),
		};
		int EndIndex[] = {0	, Dimension , 2*Dimension ,
							3 *  Dimension , 4 * Dimension};
		int SizeIndex[] = { Dimension, Dimension, Dimension, Dimension };
		int DimensionFaces[] = { 0	, Dimension * Dimension, (2*Dimension ) * Dimension ,
							(3 * Dimension ) * Dimension};
		for (size_t kk = 0; kk < 4; kk++)
		{
			for (size_t ii = 0; ii < Dimension; ii++)
			{
				for (size_t jj = 0; jj < Dimension; jj++)
				{
					glm::vec2 TexCord = CalcTexcoords(Dimension - 1, Dimension - 1, Dimension - 1, kk, jj, ii);
					glm::vec3 Positions = CalculatePosition(Dimension - 1, Dimension - 1, Dimension - 1,
															DimensionSize, DimensionSize,DimensionSize, kk, jj,ii);
					Vertex TempVertex = { Positions,Colors,TexCord,Normals[kk] };
					VertexOfCube.push_back(TempVertex);
				}
			}
		}
			//Need to Create ALgorithm to make the Indicies of the Box
			//For each of the four size
		for (size_t kk = 0; kk < 4; kk++)
		{
			for (size_t ii = 0; ii < Dimension-1;ii++)
			{
				for (size_t jj = 0; jj < Dimension-1; jj++)
				{
					GLuint TopRight = (ii * Dimension) + jj + DimensionFaces[kk];
					GLuint TopLeft = TopRight + 1;
					GLuint BottomRight = ((ii + 1) * Dimension) + jj + DimensionFaces[kk];
					GLuint BottomLeft = BottomRight + 1;
					IndecesOfCube.push_back(TopLeft);
					IndecesOfCube.push_back(BottomLeft);
					IndecesOfCube.push_back(TopRight);
					IndecesOfCube.push_back(TopRight);
					IndecesOfCube.push_back(BottomLeft);
					IndecesOfCube.push_back(BottomRight);
				}
			}
		}
		this->set(VertexOfCube, IndecesOfCube);
	}
private:
	glm::vec3 CalculatePosition(int Length, int Height, int width,
								float LengthSize, float HeightSize, float WidthSize,
								int Mode,int IndexX, int IndexY)
	{
		glm::vec3 TempPos = glm::vec3(1.f,1.f,1.f);
		switch (Mode)
		{
		case 0 :
			TempPos.x = float(HeightSize) * float(Height -2 * IndexX)/ float(2*Height);
			TempPos.y = float(WidthSize) * float(width - 2 * IndexY) / float(2 * width);
			TempPos.z = float(LengthSize) /2;
			break;
		case 1:
			TempPos.x = -1 * float(WidthSize) / 2;
			TempPos.y = float(HeightSize) * float(Height - 2 * IndexY) / float(2 * Height);
			TempPos.z = float(LengthSize) * float(Length - 2 * IndexX ) / float(2 * Length);
			break;
		case 2:
			TempPos.x = float(HeightSize) * float( 2 * IndexX - Height) / float(2 * Height);
			TempPos.y = -1 * float(WidthSize) * float(2 * IndexY - width) / float(2 * width);
			TempPos.z = -1 * float(LengthSize) / 2;
			break;
		case 3:
			TempPos.x = float(WidthSize) / 2;
			TempPos.y = float(HeightSize) * float(Height - 2 * IndexY) / float(2 * Height);
			TempPos.z = -1 * float(LengthSize) * float(Length - 2 * IndexX) / float(2 * Length);
			break;
		}
		return TempPos;
	}
	glm::vec2 CalcTexcoords(int Width, int Length, int Height, int Mode, int ii, int jj)
	{
		glm::vec2 TempTexCoord = glm::vec2(1.f,1.f);
		switch (Mode)
		{
		case 0:
		case 2:
			TempTexCoord.x = (float)(Height - ii) / ((float)Height);
			TempTexCoord.y = (float)(Width - jj) / ((float)Width);
			break;
		case 1:
		case 3:
			TempTexCoord.x = (float)(Height - ii) / ((float)Height);
			TempTexCoord.y = (float)(Length - jj) / ((float)Length);
			break;
		}
		return TempTexCoord;
	}
}; 

class CustomObject : public Primitive
{
public:
	CustomObject(const char* filename)
		:Primitive()
	{
		std::string temp = "";
		std::string src = "";
		std::vector<glm::vec3> PositionsFound;
		std::vector<glm::vec2> TexCoordsFound;
		std::vector<glm::vec3> NormalsFound;
		glm::vec3 ColorDetermined = glm::vec3(1.f,1.f,1.f);
		std::ifstream in_file;
		in_file.open(filename);
		
		if (in_file.is_open())
		{
			while (std::getline(in_file, temp))
			{
				if (temp.find("v ") == 0)
				{
					std::vector<std::string> out;
					ReturnStringArray(temp,' ',out);
					PositionsFound.push_back(glm::vec3(std::atof(out[1].c_str()), std::atof(out[2].c_str()), std::atof(out[3].c_str())));
				}
				else if (temp.find("vt ") == 0)
				{
					std::vector<std::string> out;
					ReturnStringArray(temp, ' ', out);
					TexCoordsFound.push_back(glm::vec2(std::atof(out[1].c_str()), std::atof(out[2].c_str())));
				}
				else if (temp.find("vn ") == 0)
				{
					std::vector<std::string> out;
					ReturnStringArray(temp, ' ', out);
					NormalsFound.push_back(glm::vec3(std::atof(out[1].c_str()), std::atof(out[2].c_str()), std::atof(out[3].c_str())));
				}
			}
		}
		
	}
private:
	void ReturnStringArray(std::string const &str, const char delim, std::vector<std::string> &out)
	{
		size_t start;
		size_t end = 0;
		while((start = str.find_first_not_of(delim, end)) != std::string::npos)
		{
			end = str.find(delim, start);
			out.push_back(str.substr(start, end - start));
		}
	}
};