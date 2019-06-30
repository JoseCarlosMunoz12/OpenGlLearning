#pragma once
#include <iostream>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <string>
#include <vector>
#include <SOIL2.h>

enum RGBA_chosen {
	RED_CHOSEN,
	GREEN_CHOSEN,
	BLUE_CHOSEN
};

class MipMap
{

private:
	std::vector<std::vector<glm::vec3>> MipMapHolder;
	int Width;
	int Height;
	int NumOfChannels;
public:
	MipMap(const char* filename)
	{
		unsigned char* image = SOIL_load_image(filename,&this->Width,&this->Height,&this->NumOfChannels, SOIL_LOAD_RGBA);
		unsigned int BytePerPixel = this->NumOfChannels;
		for (size_t ii = 0; ii < this->Width; ii++)
		{
			std::vector<glm::vec3> TempVec;
			for (size_t jj = 0; jj < this->Height; jj++)
			{
				glm::vec3 TempVec3;
				unsigned char* PixelOffset = image + (ii + jj * Height) * BytePerPixel;
				TempVec3.r = (int)PixelOffset[0];
				TempVec3.g = (int)PixelOffset[1];
				TempVec3.b = (int)PixelOffset[2];
				TempVec.push_back(TempVec3);
			}
			this->MipMapHolder.push_back(TempVec);
		}
		SOIL_free_image_data(image);
	}

	~MipMap()
	{

	}
	float ReturnValue(float XPos, float YPos, float WidthPos, float HeightPos,  RGBA_chosen ColorChosen)
	{
		unsigned int XPosConv = XPos / WidthPos * this->Width;
		unsigned int YPosConv = YPos / HeightPos * this->Height;
		switch (ColorChosen)
		{
		case RED_CHOSEN:
			return this->MipMapHolder[XPosConv][YPosConv].r;
			break;
		case GREEN_CHOSEN:
			return this->MipMapHolder[XPosConv][YPosConv].g;
			break;
		case BLUE_CHOSEN:
			return this->MipMapHolder[XPosConv][YPosConv].b;
			break;
		default:
			return 0.f;
		}

	}
};
