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
	std::vector<std::vector<float>> MipMapHolder;
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
		this->MapWidth = WidthForMap ;
		this->MapHeigth = HeightForMap ;
		this->MaxHeight = MaxHeightChoosen;
		unsigned char* image = SOIL_load_image(filename,&this->Width,&this->Height,&this->NumOfChannels, SOIL_LOAD_L);
		RecordWidth = this->Width;
		RecordHeight = this->Height;
		unsigned int BytePerPixel = 1;
		for (size_t ii = 0; ii < this->Height; ii++)
		{
			std::vector<float> TempVec;
			for (size_t jj = 0; jj < this->Width; jj++)
			{
				float TempVec3;
				unsigned char* PixelOffset = image + (jj + ii * this->Width) * BytePerPixel;
				TempVec3 = (float)PixelOffset[0];
				TempVec.push_back(TempVec3);
			}
			this->MipMapHolder.push_back(TempVec);
		}
		SOIL_free_image_data(image);
	}

	~MipMap()
	{
	}
	float ReturnValue(float XPos, float YPos)
	{
		float MidPointWidth = this->MapWidth / 2;
		float MidPointHeight = this->MapHeigth / 2;

		if (XPos > MidPointWidth)
		{
			XPos -= MidPointWidth;
		}
		else if (XPos < -1 * MidPointWidth)
		{
			XPos += -1 * MidPointWidth;
		}

		if (YPos > MidPointHeight)
		{
			YPos -= MidPointHeight;
		}
		else if (YPos < -1 * MidPointHeight)
		{
			YPos += -1 * MidPointHeight;
		}
		int XPosConv = (int)(XPos + MidPointWidth) / this->MapWidth * this->Width ;
		int YPosConv = (int)(YPos + MidPointWidth) / this->MapHeigth * this->Height ;
		XPosConv = glm::clamp(XPosConv, 0, this->Width - 1);
		YPosConv = glm::clamp(YPosConv, 0, this->Height - 1);
		return this->MipMapHolder[YPosConv][XPosConv] / 255.f * this->MaxHeight;
	}
	glm::vec3 ReturnVecVal(float XPos, float Ypos)
	{
		float ZVal = ReturnValue(XPos, Ypos);
		return glm::vec3(XPos, Ypos, ZVal);
	}
	int ReturnColorChosen(float XPos, float YPos)
	{
		unsigned int XPosConv = (int)(XPos + this->MapWidth / 2) / this->MapWidth * this->Width;
		unsigned int YPosConv = (int)(YPos + this->MapHeigth / 2) / this->MapHeigth * this->Height;
		return this->MipMapHolder[YPosConv][XPosConv];
	}
	void GetVertsAndInd(std::vector<glm::vec3> &Verts, std::vector<int> &Ind)
	{
		//Get Vertices of the world
		for (int ii = 0; ii < this->MapWidth; ii++)
		{
			for (int jj = 0; jj < this->MapHeigth; jj++)
			{
				float X = ((float)ii - MapWidth / 2);
				float Y = ((float)jj - MapHeigth / 2);
				Verts.push_back(ReturnVecVal(X, Y));
			}
		}
		for (int jj = 0; jj < MapWidth - 1; jj++)
		{
			for (int ii = 0; ii < MapHeigth - 1; ii++)
			{
				GLuint TopLeft = (jj * MapHeigth) + ii;
				GLuint TopRight = TopLeft + 1;
				GLuint BottomLeft = ((jj + 1) * MapWidth) + ii;
				GLuint BottomRight = BottomLeft + 1;
				Ind.push_back(BottomRight);
				Ind.push_back(BottomLeft);
				Ind.push_back(TopRight);
				Ind.push_back(TopRight);
				Ind.push_back(BottomLeft);
				Ind.push_back(TopLeft);
			}
		}
	}
	float GetMaxDim()
	{
		if (MapWidth > MapHeigth)
			return MapWidth;
		return MapHeigth;
	}
};
