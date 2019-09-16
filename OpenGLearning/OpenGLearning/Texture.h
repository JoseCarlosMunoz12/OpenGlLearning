#pragma once
#include <iostream>
#include <string>

#include <glew.h>
#include <glfw3.h>
#include <SOIL2.h>
class GeneralTextInfo
{
protected:
	GLuint ID;
	const char* Name;
	unsigned int Type;
public:
	GeneralTextInfo( const char* NewName,GLenum NewType)
		:Name(NewName),Type(NewType)
	{
	}
	void Bind(const GLint texture_unit)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(this->Type, this->ID);
	}
	void UnBind()
	{
		glActiveTexture(0);
		glBindTexture(this->Type, 0);
	}
	const char* GiveChar()
	{
		return this->Name;
	}
};

class Texture : public GeneralTextInfo
{
private:
	int width;
	int height;
	int NumOfChannels;
	const char* Name;
	unsigned char* ImageRGB;
public:
	Texture(const char* fileName, GLenum type, GLenum ColorType)
		:GeneralTextInfo(fileName,type)
	{
		this->Type = type;
		this->Name = fileName;

		unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, &this->NumOfChannels, SOIL_LOAD_RGBA);
		this->ImageRGB = image;
		glGenTextures(1, &this->ID);
		glBindTexture(type, this->ID);
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
		glDeleteTextures(1,&this->ID);
	}

	inline GLuint getID() const {return this->ID;}

};
