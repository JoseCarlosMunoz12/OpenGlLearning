#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\type_ptr.hpp>

enum ShaderType
{
	STATIC = 1,
	ANIM
};
class Shader
{
private:
	//Member variables
	GLuint id;
	int ShaderId;
	int versionMajor;
	int versionMinor;
	const char* TextName;
	std::string FolderChosen = "Shaders/";
	//Private Functions
	std::string loadShaderSource(const char* fileName)
	{
		std::string temp = "";
		std::string src = "";
		std::ifstream in_file;
		std::string Temp = FolderChosen + fileName;
		this->TextName = fileName;
		//Vertex
		in_file.open(Temp.c_str());

		if (in_file.is_open())
		{
			while (std::getline(in_file, temp))
			{
				src += temp + "\n";
			}

		}
		else
		{
			std::cout << "ERROR::SHADERS::COULD_NOT_OPEN_FILE :"<<fileName << "\n";
		}
		in_file.close();
		std::string versionNr = 
			std::to_string(versionMajor) +
			std::to_string(versionMinor) +
			"0";
		 src.replace(src.find("#version"), 12, ("#version " + versionNr));
		return src;
	}
	GLuint loadShader(GLenum type, const char* fileName)
	{
		bool loadSuccess = true;
		char infolog[512];
		GLint success;

		GLuint shader = glCreateShader(type);
		std::string str_src = this->loadShaderSource(fileName);
		const GLchar* src = str_src.c_str();
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infolog);
			std::cout << "ERROR::SHADERS::COULD_NOT_COMPILE_SHADER" << fileName << "\n";
			std::cout << infolog << "\n";
		}
		return shader;
	}
	void linkProgram( GLuint vertexShader,GLuint geometryShader, GLuint fragmentShader)
	{
		char infolog[512];
		GLint success;
		this->id = glCreateProgram();
		glAttachShader(this->id, vertexShader);
		if (geometryShader)
		{
			glAttachShader(this->id, geometryShader);
		}
		glAttachShader(this->id, fragmentShader);
		glLinkProgram(this->id);
		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->id, 512, NULL, infolog);
			std::cout << "ERROR::SHADERS::COULD_NOT_LINK_Program" << "\n";
			std::cout << infolog << "\n";

		}
		glUseProgram(0);
	}
public:
	//Constructors and Destructors
	Shader(int ShaderChosenId, ShaderType ShdType,const int versionMajor,const int versionMinor,const char* vertexFile ,
		const char* fragmentFile, const char* geometryFile = "")
		:versionMajor(versionMajor),versionMinor(versionMinor),ShaderId(ShaderChosenId)
	{
		switch (ShdType)
		{
		case STATIC:
			this->FolderChosen += "StaticShaders/";
			break;
		case ANIM:
			this->FolderChosen += "AnimShaders/";
			break;
		}
		GLuint vertexShader = 0;
		GLuint geometryShader = 0;
		GLuint fragmentShader = 0;
		vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
		if (geometryFile != "")
		{
			geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);
		}
		fragmentShader = loadShader(GL_FRAGMENT_SHADER,fragmentFile);
		this->linkProgram(vertexShader, fragmentShader, geometryShader);
		//END

		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);

		glUseProgram(0);
	}
	~Shader()
	{
		glDeleteProgram(this->id);
	}
	//Set uniform functions
	void use()
	{
		glUseProgram(this->id);
	}
	void unuse()
	{
		glUseProgram(0);
	}

	void set1i(GLint value, const GLchar* Name)
	{
		this->use();
		glUniform1i(glGetUniformLocation(this->id, Name), value);
		this->unuse();
	}

	void setVec1f(GLfloat value, const GLchar* Name)
	{
		this->use();

		glUniform1f(glGetUniformLocation(this->id, Name), value);

		this->unuse();
	}
	void setVec2f(glm::fvec2 value, const GLchar* Name)
	{
		this->use();

		glUniform2fv(glGetUniformLocation(this->id, Name), 1, glm::value_ptr(value));

		this->unuse();
	}
	void setVec3f(glm::fvec3 value,const GLchar* Name)
	{
		this->use();

		glUniform3fv(glGetUniformLocation(this->id, Name), 1, glm::value_ptr(value));

		this->unuse();
	}
	void setVec4f(glm::vec4 value, const GLchar* Name, GLboolean transpose = GL_FALSE)
	{
		this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->id, Name), 1, transpose, glm::value_ptr(value));

		this->unuse();
	}
	void setMat3fv(glm::mat3 value, const GLchar* Name, GLboolean transpose = GL_FALSE)
	{
		this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->id, Name), 1, transpose, glm::value_ptr(value));

		this->unuse();
	}
	void setMat4fv(glm::mat4 value, const GLchar* Name,GLboolean transpose = GL_FALSE)
	{
		this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->id,Name), 1, transpose, glm::value_ptr(value));

		this->unuse();
	}
	const char* GetName()
	{
		return this->TextName;
	}
	int GetShaderId()
	{
		return this->ShaderId;
	}
};
