#pragma once
#include <iostream>
#include <string>

#include <glew.h>
#include <glfw3.h>
#include <SOIL2.h>

enum RGBA_chosen {
	RED_CHOSEN,
	GREEN_CHOSEN,
	BLUE_CHOSEN,
	AlPHA_CHOSEN
};

class Texture
{
private:
	GLuint id;
	int width;
	int height;
	int NumOfChannels;
	unsigned int type;
	const char* Name;
	unsigned char* ImageRGB;
public:
	Texture(const char* fileName, GLenum type, GLenum ColorType)
	{
		this->type = type;
		this->Name = fileName;

		unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, &this->NumOfChannels, SOIL_LOAD_RGBA);
		this->ImageRGB = image;
		glGenTextures(1, &this->id);
		glBindTexture(type, this->id);
		std::cout << fileName << "  " << this->NumOfChannels << "\n";
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image)
		{
			glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, ColorType, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type);
		}
		else
		{
			std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED" << fileName << "\n";
		}
		glActiveTexture(0);
		glBindTexture(type, 0);
		SOIL_free_image_data(image);
	}
	~Texture()
	{
		glDeleteTextures(1,&this->id);
	}

	inline GLuint getID() const {return this->id;}

	void bind(const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(this->type, this->id);
	}

	void unbind()
	{
		glActiveTexture(0);
		glBindTexture(this->type, 0);
	}
	const char* GiveChar()
	{
		return this->Name;
	}
	int GetImageRGBInfo(float XPos, float YPos, float WidthPos,float HeightPOs, RGBA_chosen ColorChosen)
	{
		
		unsigned int BytePerPixel = this->NumOfChannels;
		unsigned int XPosConv;
		unsigned int YPosConv;
		unsigned char* PixelOffset = this->ImageRGB + (XPosConv +YPosConv * this->height ) * BytePerPixel;

		switch (ColorChosen)
		{
			case RED_CHOSEN:
				return (int)PixelOffset[0];
				break;
			case GREEN_CHOSEN:
				return (int)PixelOffset[1];
				break;
			case BLUE_CHOSEN:
				return (int)PixelOffset[3];
				break;
			case AlPHA_CHOSEN:
				return this->NumOfChannels >= 4 ? (int)PixelOffset[3] : int(0xff);
				break;
			default:
				return 0;
		}
		return 0;
	}
};