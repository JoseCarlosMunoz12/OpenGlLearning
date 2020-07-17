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

	inline GLuint getID() const { return this->ID; }
};

class Texture : public GeneralTextInfo
{
private:
	int width;
	int height;
	int NumOfChannels;
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


};

class ShadowTex:public GeneralTextInfo
{
	GLuint FrameBuffer;
public:
	ShadowTex(const char* NewName)
		:GeneralTextInfo(NewName, GL_TEXTURE_2D)
	{
		this->ID = 0;
		this->FrameBuffer = 0;
	}
	~ShadowTex()
	{
		if (this->FrameBuffer != 0)
		{
			glDeleteFramebuffers(1, &this->FrameBuffer);
		}
		if (this->ID!= 0)
		{
			glDeleteFramebuffers(1, &this->ID);
		}
	}
	void Init(unsigned int WindowWidth, unsigned int WindowHeight)
	{
		//Create the Frame Buffer
		glGenFramebuffers(1, &this->FrameBuffer);
		//Create The Depth Buffer
		glGenTextures(1, &this->ID);
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float BorderColor[] = { 1.f,1.f,1.f,1.f };
		glBindFramebuffer(GL_FRAMEBUFFER, this->FrameBuffer);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BorderColor);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->ID, 0);

		//Disable Writes to the color buffer
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void WriteToBuffer(unsigned int WindowWidth, unsigned int WindowHeight,
		Shader* DepthShader,glm::mat4 LightSpaceMatrix)
	{
		DepthShader->use();
		DepthShader->setMat4fv(LightSpaceMatrix, "LightSpaceMatrix");
		glViewport(0, 0, WindowWidth, WindowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, this->FrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
};