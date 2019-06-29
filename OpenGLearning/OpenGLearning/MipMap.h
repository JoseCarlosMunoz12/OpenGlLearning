#pragma once
#include <iostream>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <string>
#include <vector>
#include <SOIL2.h>


class MipMap
{

private:
	std::vector<std::vector<glm::vec3>> MipMapHolder;
	int Width;
	int Height;
	int NumOfChannels;

	MipMap(const char* filename)
	{
		unsigned char* image = SOIL_load_image(filename,&this->Width,&this->Height,&this->NumOfChannels, SOIL_LOAD_RGBA);
		unsigned int BytePerPixel = this->NumOfChannels;
		for (size_t ii = 0; ii < this->Width; ii++)
		{
			std::vector<glm::vec3> TempVec;
			for (size_t jj = 0; jj < this->Height; jj++)
			{
				glm::vec4 TempVec4;
				unsigned char* PixelOffset = image + (ii + jj * Height) * BytePerPixel;
				
			}
		}

	}

	~MipMap()
	{

	}
	glm::vec3 ReturnValue(float XPos, float YPos, float WidthPos)
	{

	}
};
