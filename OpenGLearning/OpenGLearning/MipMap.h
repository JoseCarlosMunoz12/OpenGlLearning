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
	int MaxHeight;
	float MapWidth;
	float MapHeigth;
public:
	MipMap(const char* filename,int& RecordWidth, int& RecordHeight, float MaxHeightChoosen,
			float WidthForMap, float HeightForMap)
	{
		this->MapWidth = WidthForMap;
		this->MapHeigth = HeightForMap;
		this->MaxHeight = MaxHeightChoosen;
		unsigned char* image = SOIL_load_image(filename,&this->Width,&this->Height,&this->NumOfChannels, SOIL_LOAD_RGBA);
		RecordWidth = this->Width;
		RecordHeight = this->Height;
		unsigned int BytePerPixel = this->NumOfChannels;
		for (size_t ii = 0; ii < this->Height; ii++)
		{
			std::vector<glm::vec3> TempVec;
			for (size_t jj = 0; jj < this->Width; jj++)
			{
				glm::vec3 TempVec3;
				unsigned char* PixelOffset = image + (ii + jj * this->Width) * BytePerPixel;
				TempVec3.r = (float)PixelOffset[0];
				TempVec3.g = (float)PixelOffset[1];
				TempVec3.b = (float)PixelOffset[2];
				TempVec.push_back(TempVec3);
			}
			this->MipMapHolder.push_back(TempVec);
		}
		SOIL_free_image_data(image);
	}

	~MipMap()
	{
	}
	float ReturnValue(float XPos, float YPos, RGBA_chosen ColorChosen = RED_CHOSEN)
	{
		float MidPointWidth = this->MapWidth / 2;
		float MidPointHeight = this->MapHeigth / 2;

		if (XPos > MidPointWidth)
		{
			XPos = MidPointWidth;
		}
		else if (XPos < -1 * MidPointWidth)
		{
			XPos = -1 * MidPointWidth;
		}

		if (YPos > MidPointHeight)
		{
			YPos > MidPointHeight;
		}
		else if (YPos < -1 * MidPointHeight)
		{
			YPos = -1 * MidPointHeight;
		}
		unsigned int XPosConv = (int)(XPos + MidPointWidth) / this->MapWidth * this->Width;
		unsigned int YPosConv = (int)(YPos + MidPointWidth) / this->MapHeigth * this->Height;
		switch (ColorChosen)
		{
		case RED_CHOSEN:
			return this->MipMapHolder[YPosConv][XPosConv].r /255.f * this->MaxHeight;
			break;
		case GREEN_CHOSEN:
			return this->MipMapHolder[YPosConv][XPosConv].g / 255.f * this->MaxHeight;
			break;
		case BLUE_CHOSEN:
			return this->MipMapHolder[YPosConv][XPosConv].b / 255.f * this->MaxHeight;
			break;
		default:
			return 0.f;
		}
	}
	int ReturnColorChosen(float XPos, float YPos, RGBA_chosen ColorChosen)
	{
		unsigned int XPosConv = (int)(XPos + this->MapWidth / 2) / this->MapWidth * this->Width;
		unsigned int YPosConv = (int)(YPos + this->MapHeigth / 2) / this->MapHeigth * this->Height;
		switch (ColorChosen)
		{
		case RED_CHOSEN:
			return this->MipMapHolder[YPosConv][XPosConv].r;
			break;
		case GREEN_CHOSEN:
			return this->MipMapHolder[YPosConv][XPosConv].g;
			break;
		case BLUE_CHOSEN:
			return this->MipMapHolder[YPosConv][XPosConv].b;
			break;
		default:
			return 0.f;
		}
	}
};
