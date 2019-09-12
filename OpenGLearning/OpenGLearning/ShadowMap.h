#pragma once
#include <iostream>
#include <string>

#include <glew.h>
#include <glfw3.h>
#include <SOIL2.h>

#include "Model.h"
#include "Shader.h"
class ShadowMapFBO
{
	GLuint FramebufferName;
	GLuint depthTexture;
public:
	ShadowMapFBO()
	{
		this->FramebufferName = 0;
		this->depthTexture = 0;
	};
	~ShadowMapFBO()
	{
		if (this->FramebufferName != 0)
		{
			glDeleteFramebuffers(1, &this->FramebufferName);
		}
		if (this->depthTexture != 0)
		{
			glDeleteFramebuffers(1, &this->depthTexture);
		}
	}
	void Init(unsigned int WindowWidth,unsigned int WindowHeight)
	{
		//Create the Frame Buffer
		glGenFramebuffers(1, &this->FramebufferName);
		//Create The Depth Buffer
		glGenTextures(1, &this->depthTexture);
		glBindTexture(GL_TEXTURE_2D, this->depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, this->FramebufferName);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture, 0);

		//Disable Writes to the color buffer
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void RenderShadowToTexture(unsigned int WindowWidth, unsigned int WindowHeight,
		Shader* DepthShader,std::vector<Model*>ToRender,
		glm::mat4 LightSpaceMatrix)
	{
		DepthShader->use();
		DepthShader->setMat4fv(LightSpaceMatrix,"LightSpaceMatrix");
		glViewport(0, 0, WindowWidth, WindowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, this->FramebufferName);
		glClear(GL_DEPTH_BUFFER_BIT);
		for (auto& ii : ToRender)
		{
			ii->RenderShadow(DepthShader);
		}
	}
	GLint GetDepthMap()
	{
		return this->depthTexture;
	}
	void BindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FramebufferName);
	}
	void BindForReading(GLenum TextureUnit)
	{
		glActiveTexture(TextureUnit);
		glBindTexture(GL_TEXTURE_2D, this->depthTexture);
	}

};